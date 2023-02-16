// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataTypes.generated.h"

UENUM()
enum EWeaponMode
{
	MeleeMode	UMETA(DisplayName = "Melee"),
	RangedMode	UMETA(DisplayName = "Ranged"),
};

USTRUCT(BlueprintType)
struct FDamageCurve
{
	GENERATED_USTRUCT_BODY()
public:
	FDamageCurve()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> Ranges;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int> Damages;
};

UENUM()
enum EBaseDamageType
{
	MeleeDamage		UMETA(DisplayName = "Melee"),
	RangedDamage	UMETA(DisplayName = "Ranged"),
};

USTRUCT(BlueprintType)
struct FSoulsDamageType
{
	GENERATED_USTRUCT_BODY()
public:
	FSoulsDamageType()
		: BaseDamageType(MeleeDamage)
		, bBleed(false)
		, bEnemyCaused(false)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBaseDamageType> BaseDamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBleed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnemyCaused;
};

/**
 * 
 */
class PROJECTALBATROSS_API WeaponDataTypes
{
public:
	WeaponDataTypes();
	~WeaponDataTypes();
};
