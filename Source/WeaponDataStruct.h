// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataTypes.h"
#include "WeaponProjectile.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "WeaponDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	
	FWeaponData()
		: WeaponID(0)
		, WeaponDisplayName("")
		, LightDamage(0)
		, HeavyDamage(0)
		, LightStaminaDrain(0)
		, HeavyStaminaDrain(0)
		, EquipTime(0)
		, UnEquipTime(0)
		, ModeSwapTime(0)
		, ZoomTime(0)
		, UnZoomTime(0)
		, TargetAcquisitionTime(0)
		, TargetAcquisitionDeltaPerShot(0)
		, MaxHorizontalRecoilAngle(0)
		, MaxVerticalRecoilAngle(0)
		, FireRate(0)
		, HipFireElixirAmmoCost(0)
		, ZoomElixirAmmoCost(0)
		, FireAnimation(nullptr)
		, WeaponBaseMesh(nullptr)
		, WeaponMeleeMesh(nullptr)
		, WeaponRangedMesh(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WeaponDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LightDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HeavyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LightStaminaDrain;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HeavyStaminaDrain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoulsDamageType MeleeDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDamageCurve HipFireDamageCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDamageCurve ZoomDamageCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoulsDamageType RangedDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<USoundCue*> FireSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EquipTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UnEquipTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ModeSwapTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZoomTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UnZoomTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetAcquisitionTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetAcquisitionDeltaPerShot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHorizontalRecoilAngle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxVerticalRecoilAngle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HipFireElixirAmmoCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ZoomElixirAmmoCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeaponProjectile> HipFireProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeaponProjectile> ZoomProjectile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* FireAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* WeaponBaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* WeaponMeleeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* WeaponRangedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> MeleeLightAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> MeleeHeavyAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> HipFireAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> RangedAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> FinisherAnimations;
};

/**
 * 
 */
class PROJECTALBATROSS_API WeaponDataStruct
{
public:
	WeaponDataStruct();
	~WeaponDataStruct();
};
