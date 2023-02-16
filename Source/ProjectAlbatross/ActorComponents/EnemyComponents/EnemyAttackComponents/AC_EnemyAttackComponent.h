// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ProjectAlbatross/WeaponData/WeaponDataTypes.h"
#include "AC_EnemyAttackComponent.generated.h"

UENUM()
enum EEnemyAttackType
{
	MeleeAttack		UMETA(DisplayName = "Melee"),
	RangedAttack	UMETA(DisplayName = "Ranged")
};

UENUM()
enum EEnemyAttackRange
{
	CloseRange		UMETA(DisplayName = "Close"),
	MediumRange		UMETA(DisplayName = "Medium"),
	FarRange		UMETA(DisplayName = "Far"),
};

UENUM()
enum EEnemyAttackSpeed
{
	FastSpeed		UMETA(DisplayName = "Fast"),
	MediumSpeed		UMETA(DisplayName = "Medium"),
	SlowSpeed		UMETA(DisplayName = "Slow"),
};

USTRUCT(BlueprintType)
struct FEnemyAttackAnimationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EEnemyAttackRange> AttackRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EEnemyAttackSpeed> AttackSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* AnimationMontage;
};

USTRUCT(BlueprintType)
struct FEnemyMeleeAttackData : public FTableRowBase
{
	GENERATED_BODY()

	FEnemyMeleeAttackData()
		: Damage(0)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FEnemyAttackAnimationData> AttackAnimations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSoulsDamageType MeleeDamageType;
};

USTRUCT()
struct FEnemyRangedAttackData
{
	GENERATED_BODY()
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_EnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_EnemyAttackComponent();

	UFUNCTION()
	virtual void EquipMeleeAttackData(FEnemyMeleeAttackData NewData);
	UFUNCTION()
    virtual void EquipRangedAttackData(FEnemyRangedAttackData NewData);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* EnemyMesh;
};
