// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectAlbatross/Actors/PlayerActors/SoulsCharacter.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<UAC_Weapon>(TEXT("WeaponComponent"));

	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base Mesh"));
	MeleeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Melee Mesh"));
	MeleeMesh->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	RangedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Ranged Mesh"));
	RangedMesh->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CurrentTargetAcquisition = 0;

	CurrentCombo = 0;

	CancelResetCombo = false;
}

void AWeaponActor::DoMeleeTraces()
{
	for (FMeleeWeaponDamageTraceSetup TraceSetup : MeleeTraces)
	{
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(CharacterMesh->GetOwner());
		FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceSetup.TraceRadius);
		TArray<FHitResult> OutResults;
		GetWorld()->SweepMultiByChannel(OutResults, TraceSetup.TraceStart->GetComponentLocation(), TraceSetup.TraceEnd->GetComponentLocation(), FQuat::Identity, ECollisionChannel::ECC_Pawn, Sphere, Params);
		for (FHitResult HitResult : OutResults)
		{
			int DamageAmount = 0;
			if (bCurrentIsHeavy)
			{
				DamageAmount = WeaponComponent->GetWeaponData().HeavyDamage;
			}
			else
			{
				DamageAmount = WeaponComponent->GetWeaponData().LightDamage;
			}
			if (!HitActors.Contains(HitResult.GetActor()))
			{
				SendDamage(HitResult.GetActor(), CalculateDamage(CurrentAnimation), WeaponComponent->GetWeaponData().MeleeDamageType);
				HitActors.Add(HitResult.GetActor());
			}
		}
	}
}

float AWeaponActor::Attack(bool IsHeavy, FAttackAnimation AnimationOverride)
{
	if (IsHeavy)
	{
		TArray<FAttackAnimation> HeavyAnimMontages = WeaponComponent->GetWeaponData().MeleeHeavyAnimations;
		bCurrentIsHeavy = true;
		if (CurrentCombo > 0)
		{
			CancelResetCombo = true;
		}
		if (CurrentCombo > HeavyAnimMontages.Num() - 1)
		{
			CurrentCombo = 0;
		}
		FTimerHandle ResetComboTimer;
		CurrentAnimation = HeavyAnimMontages[CurrentCombo];
		GetWorld()->GetTimerManager().SetTimer(ResetComboTimer, this, &AWeaponActor::ResetCombo, HeavyAnimMontages[CurrentCombo].Animation->GetPlayLength() / HeavyAnimMontages[CurrentCombo].Animation->RateScale, false);
		CurrentCombo++;
		return CharacterMesh->AnimScriptInstance->Montage_Play(HeavyAnimMontages[CurrentCombo - 1].Animation);
		//SendAnimMontage(WeaponComponent->GetWeaponData().MeleeHeavyAnimations[0]);
	}
	else
	{
		FAttackAnimation AnimationToPlay;
		if (AnimationOverride.Animation == nullptr)
		{
			TArray<FAttackAnimation> LightAnimMontages = WeaponComponent->GetWeaponData().MeleeLightAnimations;
			bCurrentIsHeavy = false;
			if (CurrentCombo > 0)
			{
				CancelResetCombo = true;
			}
			if (CurrentCombo > LightAnimMontages.Num() - 1)
			{
				CurrentCombo = 0;
			}
			AnimationToPlay = LightAnimMontages[CurrentCombo];
		}
		else
		{
			AnimationToPlay = AnimationOverride;
		}
		FTimerHandle ResetComboTimer;
		CurrentAnimation = AnimationToPlay;
		GetWorld()->GetTimerManager().SetTimer(ResetComboTimer, this, &AWeaponActor::ResetCombo, AnimationToPlay.Animation->GetPlayLength() / AnimationToPlay.Animation->RateScale, false);
		const float AnimTime =  CharacterMesh->AnimScriptInstance->Montage_Play(AnimationToPlay.Animation);
		CurrentCombo++;
		return AnimTime;
		//SendAnimMontage(WeaponComponent->GetWeaponData().MeleeLightAnimations[0]);
	}
}

float AWeaponActor::Fire(bool IsZoomed)
{
	TSubclassOf<AWeaponProjectile> ProjectileToSpawn;
	if (IsZoomed)
	{
		ProjectileToSpawn = WeaponComponent->GetWeaponData().ZoomProjectile;
	}
	else
	{
		ProjectileToSpawn = WeaponComponent->GetWeaponData().HipFireProjectile;
	}
	ASoulsCharacter* Character = Cast<ASoulsCharacter>(CharacterMesh->GetOwner());
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	GetWorld()->LineTraceSingleByChannel(HitResult, Character->Camera->GetComponentLocation(), Character->LockOnComponent->GetCurrentLockOnPoint()->GetActorLocation(), ECollisionChannel::ECC_Pawn, Params);
	FRotator Rotation;
	if (HitResult.bBlockingHit)
	{
		Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), HitResult.Location);
	}
	else
	{
		Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), HitResult.TraceEnd);
	}

	if (WeaponComponent->GetWeaponData().ProjectileType == SingleProjectile)
	{
		ShootSingleProjectile(Character, IsZoomed, Rotation, ProjectileToSpawn);
	}
	else
	{
		ShootSpreadProjectile(Character, IsZoomed, Rotation, ProjectileToSpawn);
	}
	
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponComponent->GetWeaponData().FireSounds[0], CharacterMesh->GetComponentLocation(), FRotator::ZeroRotator, 1, 1, 0);
	return 0;
}

void AWeaponActor::ShootSingleProjectile(AActor* Character, bool IsZoomed, FRotator Rotation, TSubclassOf<AWeaponProjectile> ProjectileToSpawn)
{
	ASoulsCharacter* SoulsCharacter = Cast<ASoulsCharacter>(Character);
	FHitResult ShootResult;
	FCollisionQueryParams ShootParams;
	ShootParams.AddIgnoredActor(Character);
	GetWorld()->LineTraceSingleByChannel(ShootResult, this->GetActorLocation(), SoulsCharacter->LockOnComponent->GetCurrentLockOnPoint()->GetActorLocation(), ECollisionChannel::ECC_Pawn, ShootParams);

	if (ShootResult.bBlockingHit)
	{
		SendDamage(ShootResult.GetActor(), WeaponComponent->GetWeaponData().HipFireDamageCurve.Damages[0], WeaponComponent->GetWeaponData().RangedDamageType);
	}

	FTransform ProjectileTransform = FTransform(GetProjectileDirection(IsZoomed, Rotation), CharacterMesh->GetOwner()->GetActorLocation(), FVector::OneVector);
	AWeaponProjectile* Projectile = GetWorld()->SpawnActorDeferred<AWeaponProjectile>(ProjectileToSpawn, ProjectileTransform, this);
	Projectile->StartingLocation = this->GetActorLocation();
	if (ShootResult.bBlockingHit)
	{
		Projectile->EndingLocation = ShootResult.Location;
	}
	else
	{
		Projectile->EndingLocation = ShootResult.TraceEnd;
	}
	Projectile->FinishSpawning(ProjectileTransform);
	/*
	CurrentTargetAcquisition += WeaponComponent->GetWeaponData().TargetAcquisitionDeltaPerShot;
	if (CurrentTargetAcquisition > 1)
	{
		CurrentTargetAcquisition = 1;
	}
	*/
}

void AWeaponActor::ShootSpreadProjectile(AActor* Character, bool IsZoomed, FRotator Rotation, TSubclassOf<AWeaponProjectile> ProjectileToSpawn)
{
	FHitResult ShootResult;
	FCollisionQueryParams ShootParams;
	ShootParams.AddIgnoredActor(Character);
	ASoulsCharacter* SoulsCharacter = Cast<ASoulsCharacter>(Character);
	GetWorld()->LineTraceSingleByChannel(ShootResult, this->GetActorLocation(), SoulsCharacter->LockOnComponent->GetCurrentLockOnPoint()->GetActorLocation(), ECollisionChannel::ECC_Pawn, ShootParams);

	if (ShootResult.bBlockingHit)
	{
		SendDamage(ShootResult.GetActor(), WeaponComponent->GetWeaponData().HipFireDamageCurve.Damages[0], WeaponComponent->GetWeaponData().RangedDamageType);
	}

	FTransform ProjectileTransform = FTransform(GetProjectileDirection(IsZoomed, Rotation), CharacterMesh->GetOwner()->GetActorLocation(), FVector::OneVector);
	AWeaponProjectile* Projectile = GetWorld()->SpawnActorDeferred<AWeaponProjectile>(ProjectileToSpawn, ProjectileTransform, this);
	Projectile->StartingLocation = this->GetActorLocation();
	if (ShootResult.bBlockingHit)
	{
		Projectile->EndingLocation = ShootResult.Location;
	}
	else
	{
		Projectile->EndingLocation = ShootResult.TraceEnd;
	}
	Projectile->FinishSpawning(ProjectileTransform);
	/*
	CurrentTargetAcquisition += WeaponComponent->GetWeaponData().TargetAcquisitionDeltaPerShot;
	if (CurrentTargetAcquisition > 1)
	{
		CurrentTargetAcquisition = 1;
	}
	*/
}

void AWeaponActor::QuickShot(FRangedAttackAnimation AnimationOverride)
{
	ASoulsCharacter* Character = Cast<ASoulsCharacter>(CharacterMesh->GetOwner());
	if (Character->GetCameraState() != CameraMeleeLockedOn)
	{
		return;
	}
	FRangedAttackAnimation AnimationToPlay;
	if (AnimationOverride.Animation == nullptr)
	{
		if (CurrentCombo > 0)
		{
			CancelResetCombo = true;
		}
		TArray<FRangedAttackAnimation> QuickShotAnimations = WeaponComponent->GetWeaponData().QuickShotAnimations;
		if (CurrentCombo > QuickShotAnimations.Num() - 1)
		{
			CurrentCombo = 0;
		}
		AnimationToPlay = QuickShotAnimations[CurrentCombo];
	}
	else
	{
		AnimationToPlay = AnimationOverride;
	}
	FTimerHandle ResetComboTimer;
	GetWorld()->GetTimerManager().SetTimer(ResetComboTimer, this, &AWeaponActor::ResetCombo, AnimationToPlay.Animation->GetPlayLength() / AnimationToPlay.Animation->RateScale, false);
	CharacterMesh->GetAnimInstance()->Montage_Play(AnimationToPlay.Animation);
	CurrentAnimation = *new FAttackAnimation();
	CurrentAnimation.Animation = AnimationToPlay.Animation;
	CurrentAnimation.DamageAmount = AnimationToPlay.DamageAmount;
	CurrentCombo++;
}

float AWeaponActor::CalculateDamage(FAttackAnimation AttackAnimation)
{
	float DamageToApply = AttackAnimation.DamageAmount;
	if (WeaponComponent->GetWeaponData().ScalingStatIDs.Num() == 0)
	{
		return DamageToApply;
	}
	TArray<int> StatIDs = WeaponComponent->GetWeaponData().ScalingStatIDs;
	const float Multiplier = 1 / StatIDs.Num();
	for (const auto StatID : StatIDs)
	{
		if (StatsComponent == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Red, TEXT("the stats component variable in the weapon has not been set"));
		}
		else
		{
			DamageToApply += (2 * StatsComponent->GetStatByID(StatID).StatLevel) * Multiplier;
		}
	}
	return DamageToApply;
}

void AWeaponActor::RefreshHitActorsArray()
{
	HitActors.Empty();
}

float AWeaponActor::GetTargetAcquisition()
{
	return CurrentTargetAcquisition;
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

float AWeaponActor::PlayAnimationMontageOnCharacter(UAnimMontage* AnimMontage)
{
	return CharacterMesh->AnimScriptInstance->Montage_Play(AnimMontage);
}

void AWeaponActor::ResetCombo()
{
	if (CancelResetCombo)
	{
		CancelResetCombo = false;
		return;
	}
	CurrentCombo = 0;
}

FRotator AWeaponActor::GetProjectileDirection(bool IsZoomed, FRotator StartingRotation)
{
	
	const float PitchValue = 0; //WeaponComponent->GetWeaponData().MaxVerticalRecoilAngle * CurrentTargetAcquisition;
	const float YawValue = 0; //UKismetMathLibrary::RandomFloatInRange(-(WeaponComponent->GetWeaponData().MaxHorizontalRecoilAngle) * CurrentTargetAcquisition, WeaponComponent->GetWeaponData().MaxHorizontalRecoilAngle) * CurrentTargetAcquisition;
	const FRotator DeltaRotation = FRotator(PitchValue, YawValue, 0);
	return StartingRotation + DeltaRotation;
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	if (CurrentTargetAcquisition > 0)
	{
		CurrentTargetAcquisition -= ((DeltaTime * CurrentTargetAcquisition) / (WeaponComponent->GetWeaponData().TargetAcquisitionTime));
		if (CurrentTargetAcquisition < 0)
		{
			CurrentTargetAcquisition = 0;
		}
	}
	*/
}

