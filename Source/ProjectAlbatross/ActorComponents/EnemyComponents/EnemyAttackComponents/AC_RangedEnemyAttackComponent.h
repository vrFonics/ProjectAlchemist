// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_EnemyAttackComponent.h"
#include "AC_RangedEnemyAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UAC_RangedEnemyAttackComponent : public UAC_EnemyAttackComponent
{
	GENERATED_BODY()
	
public:
	virtual void EquipRangedAttackData(FEnemyRangedAttackData NewData) override;

	UPROPERTY()
	FEnemyRangedAttackData RangedAttackData;
};
