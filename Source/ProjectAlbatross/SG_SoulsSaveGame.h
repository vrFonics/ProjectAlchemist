// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/PlayerComponents/AC_StatsComponent.h"
#include "GameFramework/SaveGame.h"
#include "SG_SoulsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API USG_SoulsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerStat> Stats;
};
