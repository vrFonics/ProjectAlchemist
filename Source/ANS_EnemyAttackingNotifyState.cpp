// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_EnemyAttackingNotifyState.h"

void UANS_EnemyAttackingNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	OwningEnemy = Cast<AEnemyBase>(MeshComp->GetOwner());
	if (OwningEnemy != nullptr)
	{
		OwningEnemy->NotifyAttackOfNewDamageEvent();	
	}
}
void UANS_EnemyAttackingNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (OwningEnemy != nullptr)
	{
		OwningEnemy->NotifyAttackOfDamageTick();
	}
}
void UANS_EnemyAttackingNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
}