// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ChangeStaggerableStatePlayer.h"

void UANS_ChangeStaggerableStatePlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	SoulsCharacter = Cast<ASoulsCharacter>(MeshComp->GetOwner());
	if (SoulsCharacter != nullptr)
	{
		PreviousStaggerableState = SoulsCharacter->StaggerableState;
		SoulsCharacter->StaggerableState = NewStaggerableState;
	}
}

void UANS_ChangeStaggerableStatePlayer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_ChangeStaggerableStatePlayer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (SoulsCharacter != nullptr)
	{
		SoulsCharacter->StaggerableState = PreviousStaggerableState;
	}
}
