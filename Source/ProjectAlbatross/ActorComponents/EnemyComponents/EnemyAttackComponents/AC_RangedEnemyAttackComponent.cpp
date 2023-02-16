// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_RangedEnemyAttackComponent.h"

void UAC_RangedEnemyAttackComponent::EquipRangedAttackData(FEnemyRangedAttackData NewData)
{
	Super::EquipRangedAttackData(NewData);
	RangedAttackData = NewData;
}
