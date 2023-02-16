// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ProjectAlbatross/Actors/EnemyActors/EnemyBase.h"
#include "ProjectAlbatross/Actors/PlayerActors/SoulsCharacter.h"
#include "ANS_ChangeStaggerableStateEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UANS_ChangeStaggerableStateEnemy : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	AEnemyBase* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EStaggerableState> NewStaggerableState;

	UPROPERTY()
	TEnumAsByte<EStaggerableState> PreviousStaggerableState;
};
