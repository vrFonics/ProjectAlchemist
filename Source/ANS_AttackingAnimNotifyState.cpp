// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackingAnimNotifyState.h"

#include "SoulsCharacter.h"

void UANS_AttackingAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	SoulsCharacter = Cast<ASoulsCharacter>(MeshComp->GetOwner());
	if (SoulsCharacter != nullptr)
	{
		SoulsCharacter->NotifyWeaponOfNewDamageEvent();
	}
}
void UANS_AttackingAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (SoulsCharacter != nullptr)
	{
		SoulsCharacter->NotifyWeaponOfDamageTick();
	}
}
void UANS_AttackingAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}