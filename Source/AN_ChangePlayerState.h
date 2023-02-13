// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulsCharacter.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ChangePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UAN_ChangePlayerState : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	
	UPROPERTY()
	ASoulsCharacter* SoulsCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESoulsCharacterState> NewState; 
};
