// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ChangeStaggerableStateEnemy.h"

void UANS_ChangeStaggerableStateEnemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Enemy = Cast<AEnemyBase>(MeshComp->GetOwner());
	if (Enemy != nullptr)
	{
		PreviousStaggerableState = Enemy->StaggerableState;
		Enemy->StaggerableState = NewStaggerableState;
	}
}

void UANS_ChangeStaggerableStateEnemy::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_ChangeStaggerableStateEnemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (Enemy != nullptr)
	{
		Enemy->StaggerableState = PreviousStaggerableState;
	}
}