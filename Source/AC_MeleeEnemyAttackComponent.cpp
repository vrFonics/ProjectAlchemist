// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_MeleeEnemyAttackComponent.h"

FEnemyMeleeAttackData UAC_MeleeEnemyAttackComponent::GetMeleeAttackData()
{
	return MeleeAttackData;
}

void UAC_MeleeEnemyAttackComponent::EquipMeleeAttackData(FEnemyMeleeAttackData NewData)
{
	Super::EquipMeleeAttackData(NewData);
	MeleeAttackData = NewData;
}
