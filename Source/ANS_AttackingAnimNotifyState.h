// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulsCharacter.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackingAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UANS_AttackingAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY()
	ASoulsCharacter* SoulsCharacter;
};
