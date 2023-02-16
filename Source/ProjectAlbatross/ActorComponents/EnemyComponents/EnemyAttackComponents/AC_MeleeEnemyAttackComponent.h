// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_EnemyAttackComponent.h"
#include "AC_MeleeEnemyAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UAC_MeleeEnemyAttackComponent : public UAC_EnemyAttackComponent
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	FEnemyMeleeAttackData GetMeleeAttackData();

public:
	virtual void EquipMeleeAttackData(FEnemyMeleeAttackData NewData) override;

	UPROPERTY()
	FEnemyMeleeAttackData MeleeAttackData;
};
