// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackActor.h"
#include "ProjectAlbatross/ActorComponents/EnemyComponents/EnemyAttackComponents/AC_RangedEnemyAttackComponent.h"
#include "RangedEnemyAttackActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API ARangedEnemyAttackActor : public AEnemyAttackActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	UAC_RangedEnemyAttackComponent* RangedAttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RangedAttackID;
};
