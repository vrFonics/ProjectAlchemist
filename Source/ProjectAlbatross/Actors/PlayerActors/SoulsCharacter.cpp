// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulsCharacter.h"

#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "ProjectAlbatross/GameModes/ProjectAlbatrossGameModeBase.h"

// Sets default values
ASoulsCharacter::ASoulsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->AddLocalOffset(FVector(0, 0, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 1.5));
	
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 4.0f;
	SpringArmComponent->CameraRotationLagSpeed = 2.0f;
	SpringArmComponent->CameraLagMaxDistance = 2000.0f;
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritYaw = true;
	
	TargetSpringArmRotation = FRotator::ZeroRotator;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SoulsCameraComponent = CreateDefaultSubobject<UAC_SoulsCameraComponent>(TEXT("SoulsCameraComponent"));
	
	bJustSwitchedTarget = false;
	bIsInRanged = false;
	bDamageable = true;

	FVector CapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
	CapsuleLocation = FVector(CapsuleLocation.X, CapsuleLocation.Y, CapsuleLocation.Z - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	GetMesh()->SetWorldLocation(CapsuleLocation);
	GetMesh()->SetRelativeRotation(FRotator(0, 0, 0));

	bInputQueueable = true;
}

void ASoulsCharacter::UpdateCameraState(ECameraState NewState)
{
	if (SpringArmSettingsByCameraState.Find(NewState) == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("WARNING - No FSpringArmSettings Structure found for camera state. Ensure all possible camera states have an FSpringArmsSettings structure associated."));
		return;
	}
	if (NewState == CameraMeleeLockedOn)
	{
		bUseControllerRotationYaw = true;
	}
	else
	{
		//Remove locked on and ranged contexts from player
		const APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		SubSystem->RemoveMappingContext(LockedOnMappingContext);
		DisengageRangedMode();
		bUseControllerRotationYaw = false;
	}
	UpdateSpringArmSettings(*SpringArmSettingsByCameraState.Find(NewState), NewState);
	CameraState = NewState;
}

ECameraState ASoulsCharacter::GetCameraState()
{
	return CameraState;
}

// Called when the game starts or when spawned
void ASoulsCharacter::BeginPlay()
{
	Super::BeginPlay();
	SoulsCharacterState = Moving;
	HealthComponent->KillDelegate.BindUObject(this, &ASoulsCharacter::Die);
	UpdateSpringArmSettings(*SpringArmSettingsByCameraState.Find(CameraMelee), CameraMelee);
	CurrentPlayerStamina = MaxPlayerStamina;
}

//TODO
//SWITCH CAMERA->GETFORWARDVECTOR() TO FORWARD VECTOR THAT IGNORES PITCH USED FOR DASH DIRECTION CALCULATIONS.
void ASoulsCharacter::MovePlayer(const FInputActionValue& Value)
{
	IntendedDirection = Value.Get<FVector2D>();
	if (SoulsCharacterState != Moving)
	{
		return;
	}
	AddMovementInput(Camera->GetForwardVector(), IntendedDirection.Y);
	AddMovementInput(Camera->GetRightVector(), IntendedDirection.X);
}

/*
void ASoulsCharacter::MoveX(float AxisValue)
{
	if (SoulsCharacterState != Moving)
	{
		return;
	}
	AddMovementInput(Camera->GetRightVector(), AxisValue);
}
*/

void ASoulsCharacter::RotateCamera(const FInputActionValue& Value)
{
	if (CameraState == CameraMeleeLockedOn)
	{
		return;
	}
	TargetSpringArmRotation.Pitch += -(Value.Get<FVector2D>().Y) * ControllerSensitivity;
	TargetSpringArmRotation.Pitch = UKismetMathLibrary::ClampAngle(TargetSpringArmRotation.Pitch, -60, 45);
	TargetSpringArmRotation.Yaw += Value.Get<FVector2D>().X * ControllerSensitivity;
}

/*
void ASoulsCharacter::RotateCameraX(float AxisValue)
{
	if (CameraState == CameraMeleeLockedOn)
	{
		return;
	}
	TargetSpringArmRotation.Yaw += AxisValue * ControllerSensitivity;
}
*/

void ASoulsCharacter::TryLockOn()
{
	if (CameraState == CameraMeleeLockedOn)
	{
		PreviousSpringArmRotation = SpringArmComponent->GetComponentRotation();
		UpdateCameraState(CameraMelee);
		const APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		SubSystem->RemoveMappingContext(LockedOnMappingContext);
		return;
	}
	if (CameraState != CameraMelee)
	{
		return;
	}
	if (LockOnComponent->LockOn(GetLocalViewingPlayerController(), Camera->GetComponentLocation()))
	{
		UpdateCameraState(CameraMeleeLockedOn);
		const APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		SubSystem->AddMappingContext(LockedOnMappingContext, 1);
		
	}
}

void ASoulsCharacter::TrySwitchLockOnTarget(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (InputVector.Length() >= LockOnSwitchMinimumDistance && !bJustSwitchedTarget)
	{
		LockOnComponent->TryChangeTarget(this->GetLocalViewingPlayerController(), InputVector, Camera->GetComponentLocation());
		bJustSwitchedTarget = true;
	}
	if (InputVector.Length() < LockOnSwitchMinimumDistance)
	{
		bJustSwitchedTarget = false;
	}
}

void ASoulsCharacter::UpdateSoulsCharacterState(ESoulsCharacterState NewState)
{
	const ESoulsCharacterState OriginalState = SoulsCharacterState;
	SoulsCharacterState = NewState;
	if ((OriginalState != Moving) && (NewState == Moving))
	{
		ProcessCurrentQueuedInput();
	}
}

void ASoulsCharacter::ToggleMenu(bool bAddMenuContext, bool bCanPlayerMove)
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (bAddMenuContext)
	{
		if (!bCanPlayerMove)
		{
			PreviousMappingContexts.Empty();
			if (SubSystem->HasMappingContext(MovingMappingContext))
			{
				PreviousMappingContexts.Add(MovingMappingContext, 0);
			}
			if (SubSystem->HasMappingContext(LockedOnMappingContext))
			{
				PreviousMappingContexts.Add(LockedOnMappingContext, 1);
			}
			if (SubSystem->HasMappingContext(RangedMappingContext))
			{
				PreviousMappingContexts.Add(RangedMappingContext, 2);
			}
			SubSystem->ClearAllMappings();
		}
		SubSystem->AddMappingContext(MenuMappingContext, 3);
	}
	else
	{
		SubSystem->RemoveMappingContext(MenuMappingContext);
		for (const TPair<class UInputMappingContext*, int> MappingContext : PreviousMappingContexts)
		{
			if (!SubSystem->HasMappingContext(MappingContext.Key))
			{
				SubSystem->AddMappingContext(MappingContext.Key, MappingContext.Value);
			}
		}
	}
}

float ASoulsCharacter::AddStamina(float StaminaToAdd)
{
	CurrentPlayerStamina += StaminaToAdd;
	if (CurrentPlayerStamina > MaxPlayerStamina)
	{
		CurrentPlayerStamina = MaxPlayerStamina;
	}
	return CurrentPlayerStamina;
}

float ASoulsCharacter::RemoveStamina(float StaminaToRemove)
{
	CurrentPlayerStamina -= StaminaToRemove;
	if (CurrentPlayerStamina < 0)
	{
		CurrentPlayerStamina = 0;
	}
	UsedStamina();
	return CurrentPlayerStamina;
}

float ASoulsCharacter::GetCurrentStamina()
{
	return CurrentPlayerStamina;
}

void ASoulsCharacter::Blink()
{
	if (SoulsCharacterState != Moving)
	{
		if (bInputQueueable)
		{
			CurrentQueuedInput = ESI_Blink;
		}
		return;
	}
	if (CurrentPlayerStamina <= 0)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(StumbleMontage);
		return;
	}
	SuccessfullyBlinked();
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(this);
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FHitResult HitResult;
	FTransform CameraTransform = Camera->GetComponentTransform();
	CameraTransform.SetRotation(FRotator(0, CameraTransform.GetRotation().Rotator().Yaw, 0).Quaternion());
	FVector TestLocation = this->GetCapsuleComponent()->GetComponentLocation() + (UKismetMathLibrary::TransformDirection(CameraTransform, GetBlinkDirection()) * 350);
	GetWorld()->SweepSingleByChannel(HitResult, this->GetCapsuleComponent()->GetComponentLocation(), TestLocation, FQuat::Identity, ECC_Visibility, Capsule, Params);
	if (HitResult.bBlockingHit)
	{
		if (HitResult.Distance != 0)
		{
			BlinkTargetLocation = HitResult.Location;
			PreviousLocation = GetActorLocation();
			TimeSinceBlinkStarted = 0;
			bDamageable = false;
			NotifyPlayerOfBlinkStarted();
			UpdateSoulsCharacterState(Blinking);
			GetWorldTimerManager().SetTimer(BlinkTimer, this, &ASoulsCharacter::FinishBlink, BlinkDuration, false);
		}
		else
		{
			return;
		}
	}
	else
	{
		BlinkTargetLocation = TestLocation;
		PreviousLocation = GetActorLocation();
		TimeSinceBlinkStarted = 0;
		bDamageable = false;
		NotifyPlayerOfBlinkStarted();
		UpdateSoulsCharacterState(Blinking);
		GetWorldTimerManager().SetTimer(BlinkTimer, this, &ASoulsCharacter::FinishBlink, BlinkDuration, false);
	}
	RemoveStamina(25);
}

void ASoulsCharacter::FinishBlink()
{
	bDamageable = true;
	NotifyPlayerOfBlinkFinished();
	if (CurrentQueuedInput == ESI_LightAttack)
	{
		WeaponHolderComponent->GetCurrentWeapon()->Attack(false, WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().OutOfDodgeLightAttackAnimation);
		UpdateSoulsCharacterState(Attacking);
		
		return;
	}
	if (CurrentQueuedInput == ESI_QuickShot)
	{
		WeaponHolderComponent->GetCurrentWeapon()->QuickShot(WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().OutOfDodgeQuickShotAnimation);
		UpdateSoulsCharacterState(Attacking);
		return;
	}
	UpdateSoulsCharacterState(Moving);
}

FVector ASoulsCharacter::GetBlinkDirection()
{
	BlinkDirection = FVector(IntendedDirection.Y, IntendedDirection.X, 0);
	if (BlinkDirection == FVector::ZeroVector)
	{
		BlinkDirection = FVector(-1, 0, 0);
		return BlinkDirection;
	}
	BlinkDirection.Normalize();
	return BlinkDirection;
}

void ASoulsCharacter::UpdateSpringArmSettings(FSpringArmSettings NewSettings, ECameraState NewState)
{
	SpringArmComponent->bEnableCameraLag = NewSettings.bEnableCameraLag;
	SpringArmComponent->CameraLagSpeed = NewSettings.CameraLagSpeed;
	SpringArmComponent->CameraRotationLagSpeed = NewSettings.CameraRotationLagSpeed;
	SpringArmComponent->CameraLagMaxDistance = NewSettings.CameraLagMaxDistance;
	SpringArmComponent->TargetArmLength = NewSettings.TargetArmLength;
	SpringArmComponent->bUsePawnControlRotation = NewSettings.bUsePawnControlRotation;

	/*
	const FQuat NewQuaternion = SpringArmComponent->GetRelativeTransform().InverseTransformRotation(SpringArmComponent->GetComponentTransform().Rotator().Quaternion());
	TargetSpringArmRotation.Pitch = NewQuaternion.Rotator().Pitch;
	SpringArmComponent->bInheritYaw = NewSettings.bInheritYaw;
	FRotator NewRotator = NewQuaternion.Rotator();
	NewRotator.Roll = 0.0f;
	if (NewState != CameraRanged || NewState != CameraZoomed)
	{
		if (NewState == CameraMelee)
		{
			if (PreviousSpringArmRotation != FRotator::ZeroRotator)
			{
				SpringArmComponent->SetWorldRotation(PreviousSpringArmRotation);
				Controller->SetControlRotation(PreviousSpringArmRotation);
			}
			else
			{
				SpringArmComponent->SetRelativeRotation(NewRotator);
			}
		}
	}
	else
	{
		if (CameraState == CameraZoomed || CameraState == CameraMeleeLockedOn)
		{
			SpringArmComponent->SetRelativeRotation(FRotator::ZeroRotator);
			GetMesh()->SetRelativeRotation(SpringArmComponent->GetRelativeRotation());
		}
	}
	SpringArmComponent->SetRelativeLocation(NewSettings.LocalSpringArmOffset);
	*/
}

void ASoulsCharacter::DoLockedCameraRotation(float DeltaTime)
{
	FRotator TargetLockedRotation = LockOnComponent->GetLockedSpringArmRotation(SpringArmComponent->GetComponentLocation());
	TargetLockedRotation.Pitch = FMath::ClampAngle(TargetLockedRotation.Pitch, -25, 45);
	//SpringArmComponent->SetWorldRotation(TargetLockedRotation);
	GetController()->SetControlRotation(FQuat::Slerp(GetControlRotation().Quaternion(), TargetLockedRotation.Quaternion(), DeltaTime * 5).Rotator());
	TargetSpringArmRotation = GetControlRotation();
	SpringArmComponent->SetRelativeRotation_Direct(TargetSpringArmRotation);
}

void ASoulsCharacter::LightAttack()
{
	if (SoulsCharacterState != Moving)
	{
		if (bInputQueueable)
		{
			CurrentQueuedInput = ESI_LightAttack;
		}
		return;
	}
	if (CurrentPlayerStamina <= 0)
	{
		return;
	}
	if (WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetMode() == true)
	{
		WeaponHolderComponent->GetCurrentWeapon()->Attack(false);
		RemoveStamina(WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().LightStaminaDrain);
		PreviousCharacterState = SoulsCharacterState;
		UpdateSoulsCharacterState(Attacking);
		SuccessfullyLightAttacked();
	}
}

void ASoulsCharacter::HeavyAttack()
{
	if (SoulsCharacterState != Moving)
	{
		if (bInputQueueable)
		{
			CurrentQueuedInput = ESI_HeavyAttack;
		}
		return;
	}
	if (CurrentPlayerStamina <= 0)
	{
		return;
	}
	if (WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetMode() == true)
	{
		WeaponHolderComponent->GetCurrentWeapon()->Attack(true);
		RemoveStamina(WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().HeavyStaminaDrain);
		PreviousCharacterState = SoulsCharacterState;
		UpdateSoulsCharacterState(Attacking);
		SuccessfullyHeavyAttacked();
	}
	else
	{
		if (WeaponHolderComponent->Fire(GetCameraState() == CameraZoomed))
		{
			if (CameraState == CameraZoomed)
			{
				SuccessfullyZoomFired();
			}
			else
			{
				SuccessfullyHipFired();
			}
		}
	}
}

void ASoulsCharacter::QuickShot()
{
	if (SoulsCharacterState != Moving)
	{
		if (bInputQueueable)
		{
			CurrentQueuedInput = ESI_QuickShot;
		}
		return;
	}
	WeaponHolderComponent->GetCurrentWeapon()->QuickShot();
	UpdateSoulsCharacterState(Attacking);
	NotifyPlayerOfQuickshot();
}

void ASoulsCharacter::SpecialRangedShot()
{
	if (SoulsCharacterState != Moving)
	{
		if (bInputQueueable)
		{
			CurrentQueuedInput = ESI_SpecialRangedShot;
		}
		return;
	}
	WeaponHolderComponent->Fire(true);
}

void ASoulsCharacter::EngageRangedMode()
{
	if (bIsInRanged)
	{
		DisengageRangedMode();
		return;
	}
	bIsInRanged = true;
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->AddMappingContext(RangedMappingContext, 2);
}

void ASoulsCharacter::DisengageRangedMode()
{
	bIsInRanged = false;
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->RemoveMappingContext(RangedMappingContext);
}

// Called every frame
void ASoulsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CameraState == CameraMelee)
	{
		SoulsCameraComponent->DoCameraLag(DeltaTime, GetInputAxisValue("RotateCameraX"), GetInputAxisValue("RotateCamera"), GetInputAxisValue("MoveX"), GetInputAxisValue("MovePlayer"), GetCapsuleComponent(),	SpringArmComponent, Camera);
		if (GetController() != nullptr)
		{
			GetController()->SetControlRotation(TargetSpringArmRotation);
		}
		SpringArmComponent->SetRelativeRotation_Direct(GetControlRotation());
	}
	else if (CameraState == CameraMeleeLockedOn)
	{
		DoLockedCameraRotation(DeltaTime);
	}
	if (SoulsCharacterState == Blinking)
	{
		TimeSinceBlinkStarted += DeltaTime;
		if (TimeSinceBlinkStarted >= BlinkDuration)
		{
			TimeSinceBlinkStarted = BlinkDuration;
		}
		SetActorLocation(FMath::Lerp(PreviousLocation, BlinkTargetLocation, TimeSinceBlinkStarted / BlinkDuration));
	}
	/*
	else if (CameraState == CameraRanged || CameraState == CameraZoomed)
	{
		FRotator RecoilTargetRotation = TargetSpringArmRotation;
		RecoilTargetRotation.Pitch += WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().MaxVerticalRecoilAngle * WeaponHolderComponent->GetCurrentWeapon()->GetTargetAcquisition();
		RecoilTargetRotation.Pitch = UKismetMathLibrary::ClampAngle(RecoilTargetRotation.Pitch, -60, 45);
		RecoilTargetRotation.Yaw += WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().MaxHorizontalRecoilAngle * WeaponHolderComponent->GetCurrentWeapon()->GetTargetAcquisition();
		GetController()->SetControlRotation(RecoilTargetRotation);
		SpringArmComponent->SetRelativeRotation(RecoilTargetRotation);
	}
	*/
}

// Called to bind functionality to input
void ASoulsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*
	InputComponent->BindAxis("MovePlayer", this, &ASoulsCharacter::MovePlayer);
	InputComponent->BindAxis("MoveX", this, &ASoulsCharacter::MoveX);
	InputComponent->BindAxis("RotateCamera", this, &ASoulsCharacter::RotateCamera);
	InputComponent->BindAxis("RotateCameraX", this, &ASoulsCharacter::RotateCameraX);
	InputComponent->BindAction("Blink", IE_Pressed, this, &ASoulsCharacter::Blink);
	InputComponent->BindAction("LockOn", IE_Pressed, this, &ASoulsCharacter::TryLockOn);
	InputComponent->BindAction("LightAttack", IE_Pressed, this, &ASoulsCharacter::LightAttack);
	InputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ASoulsCharacter::HeavyAttack);
	*/
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(MovingMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(PlayerMoveAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::MovePlayer);
	EnhancedInputComponent->BindAction(PlayerMoveAction, ETriggerEvent::None, this, &ASoulsCharacter::MovePlayer);
	EnhancedInputComponent->BindAction(PlayerLookAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::RotateCamera);
	EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::Blink);
	EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::LightAttack);
	EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::HeavyAttack);
	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::TryLockOn);
	EnhancedInputComponent->BindAction(SwitchLockOnTargetAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::TrySwitchLockOnTarget);
	EnhancedInputComponent->BindAction(EngageRangedModeAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::EngageRangedMode);
	/*
	 *
	EnhancedInputComponent->BindAction(DisengageRangedModeAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::DisengageRangedMode);
	*/
	EnhancedInputComponent->BindAction(QuickShotAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::QuickShot);
	EnhancedInputComponent->BindAction(SpecialRangedShotAction, ETriggerEvent::Triggered, this, &ASoulsCharacter::SpecialRangedShot);
}

void ASoulsCharacter::ProcessCurrentQueuedInput()
{
	switch (CurrentQueuedInput)
	{
	case ESI_LightAttack : LightAttack();
		break;
	case ESI_HeavyAttack : HeavyAttack();
		break;
	case ESI_Blink : Blink();
		break;
	case ESI_QuickShot : QuickShot();
		break;
	case ESI_SpecialRangedShot : SpecialRangedShot();
	default:
		break;
	}
	bInputQueueable = true;
	CurrentQueuedInput = ESI_None;
}

