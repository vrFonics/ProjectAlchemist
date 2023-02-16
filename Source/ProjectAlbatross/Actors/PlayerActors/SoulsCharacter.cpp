// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulsCharacter.h"

#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	FVector CapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
	CapsuleLocation = FVector(CapsuleLocation.X, CapsuleLocation.Y, CapsuleLocation.Z - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	GetMesh()->SetWorldLocation(CapsuleLocation);
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	bInputQueueable = true;
}

void ASoulsCharacter::UpdateCameraState(ECameraState NewState)
{
	if (SpringArmSettingsByCameraState.Find(NewState) == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("WARNING - No FSpringArmSettings Structure found for camera state. Ensure all possible camera states have an FSpringArmsSettings structure associated."));
		return;
	}
	if (NewState == CameraMeleeLockedOn || NewState == CameraRanged || NewState == CameraZoomed)
	{
		bUseControllerRotationYaw = true;
	}
	else
	{
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
void ASoulsCharacter::MoveY(float AxisValue)
{
	if (SoulsCharacterState != Moving)
	{
		return;
	}
	AddMovementInput(FRotator(0, Camera->GetComponentRotation().Yaw, 0).Vector(), AxisValue);
}

void ASoulsCharacter::MoveX(float AxisValue)
{
	if (SoulsCharacterState != Moving)
	{
		return;
	}
	AddMovementInput(Camera->GetRightVector(), AxisValue);
}

void ASoulsCharacter::RotateCameraY(float AxisValue)
{
	if (CameraState == CameraMeleeLockedOn)
	{
		return;
	}
	TargetSpringArmRotation.Pitch += AxisValue * ControllerSensitivity;
	TargetSpringArmRotation.Pitch = UKismetMathLibrary::ClampAngle(TargetSpringArmRotation.Pitch, -60, 45);
}

void ASoulsCharacter::RotateCameraX(float AxisValue)
{
	if (CameraState == CameraMeleeLockedOn)
	{
		return;
	}
	TargetSpringArmRotation.Yaw += AxisValue * ControllerSensitivity;
}

void ASoulsCharacter::TryLockOn()
{
	if (CameraState == CameraMeleeLockedOn)
	{
		PreviousSpringArmRotation = SpringArmComponent->GetComponentRotation();
		UpdateCameraState(CameraMelee);
		return;
	}
	if (CameraState == CameraRanged)
	{
		UpdateCameraState(CameraZoomed);
	}
	else if (CameraState == CameraZoomed)
	{
		UpdateCameraState(CameraRanged);
	}
	if (CameraState != CameraMelee)
	{
		return;
	}
	if (LockOnComponent->LockOn(GetLocalViewingPlayerController(), Camera->GetComponentLocation()))
	{
		UpdateCameraState(CameraMeleeLockedOn);
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
	FVector TestLocation = this->GetCapsuleComponent()->GetComponentLocation() + (UKismetMathLibrary::TransformDirection(FTransform(FRotator(0, Camera->GetComponentRotation().Yaw, 0).Quaternion(), FVector::ZeroVector, FVector::ZeroVector), GetBlinkDirection()) * 350);
	GetWorld()->SweepSingleByChannel(HitResult, this->GetCapsuleComponent()->GetComponentLocation(), TestLocation, FQuat::Identity, ECC_Visibility, Capsule, Params);
	if (HitResult.bBlockingHit)
	{
		if (HitResult.Distance != 0)
		{
			SetActorLocation(HitResult.Location);
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
		SetActorLocation(TestLocation);
		UpdateSoulsCharacterState(Blinking);
		GetWorldTimerManager().SetTimer(BlinkTimer, this, &ASoulsCharacter::FinishBlink, BlinkDuration, false);
	}
	RemoveStamina(25);
}

void ASoulsCharacter::FinishBlink()
{
	UpdateSoulsCharacterState(Moving);
}

FVector ASoulsCharacter::GetBlinkDirection()
{
	BlinkDirection = FVector(GetInputAxisValue("MoveY"), GetInputAxisValue("MoveX"), 0);
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
}

void ASoulsCharacter::DoLockedCameraRotation()
{
	FRotator TargetLockedRotation = LockOnComponent->GetLockedSpringArmRotation(SpringArmComponent->GetComponentLocation());
	TargetLockedRotation.Pitch = FMath::ClampAngle(TargetLockedRotation.Pitch, -25, 45);
	//SpringArmComponent->SetWorldRotation(TargetLockedRotation);
	GetController()->SetControlRotation(TargetLockedRotation);
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
		WeaponHolderComponent->Attack(false);
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
		WeaponHolderComponent->Attack(true);
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

// Called every frame
void ASoulsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CameraState == CameraMelee)
	{
		SoulsCameraComponent->DoCameraLag(DeltaTime, GetInputAxisValue("RotateCameraX"), GetInputAxisValue("RotateCameraY"), GetInputAxisValue("MoveX"), GetInputAxisValue("MoveY"), GetCapsuleComponent(),	SpringArmComponent, Camera);
		GetController()->SetControlRotation(TargetSpringArmRotation);
		SpringArmComponent->SetRelativeRotation(TargetSpringArmRotation);
	}
	else if (CameraState == CameraMeleeLockedOn)
	{
		DoLockedCameraRotation();
		FVector2D InputVector;
		InputVector.X = GetInputAxisValue("RotateCameraX");
		InputVector.Y = GetInputAxisValue("RotateCameraY");
		InputVector.Normalize();
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
	else if (CameraState == CameraRanged || CameraState == CameraZoomed)
	{
		FRotator RecoilTargetRotation = TargetSpringArmRotation;
		RecoilTargetRotation.Pitch += WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().MaxVerticalRecoilAngle * WeaponHolderComponent->GetCurrentWeapon()->GetTargetAcquisition();
		RecoilTargetRotation.Pitch = UKismetMathLibrary::ClampAngle(RecoilTargetRotation.Pitch, -60, 45);
		RecoilTargetRotation.Yaw += WeaponHolderComponent->GetCurrentWeapon()->WeaponComponent->GetWeaponData().MaxHorizontalRecoilAngle * WeaponHolderComponent->GetCurrentWeapon()->GetTargetAcquisition();
		GetController()->SetControlRotation(RecoilTargetRotation);
		SpringArmComponent->SetRelativeRotation(RecoilTargetRotation);
	}
}

// Called to bind functionality to input
void ASoulsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveY", this, &ASoulsCharacter::MoveY);
	InputComponent->BindAxis("MoveX", this, &ASoulsCharacter::MoveX);
	InputComponent->BindAxis("RotateCameraY", this, &ASoulsCharacter::RotateCameraY);
	InputComponent->BindAxis("RotateCameraX", this, &ASoulsCharacter::RotateCameraX);
	InputComponent->BindAction("Blink", IE_Pressed, this, &ASoulsCharacter::Blink);
	InputComponent->BindAction("LockOn", IE_Pressed, this, &ASoulsCharacter::TryLockOn);
	InputComponent->BindAction("LightAttack", IE_Pressed, this, &ASoulsCharacter::LightAttack);
	InputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ASoulsCharacter::HeavyAttack);
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
	default:
		break;
	}
	bInputQueueable = true;
	CurrentQueuedInput = ESI_None;
}

