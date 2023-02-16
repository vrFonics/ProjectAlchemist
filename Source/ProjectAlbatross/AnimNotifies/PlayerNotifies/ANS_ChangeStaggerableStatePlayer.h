// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ProjectAlbatross/Actors/PlayerActors/SoulsCharacter.h"
#include "ANS_ChangeStaggerableStatePlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UANS_ChangeStaggerableStatePlayer : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	ASoulsCharacter* SoulsCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EStaggerableState> NewStaggerableState;

	UPROPERTY()
	TEnumAsByte<EStaggerableState> PreviousStaggerableState;
};
