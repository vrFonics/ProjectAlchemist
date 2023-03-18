// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectAlbatross/Actors/PlayerActors/SoulsCharacter.h"
#include "GI_SoulsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API UGI_SoulsGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void SaveGame(FString SaveSlot, ASoulsCharacter* SoulsCharacter);

	UFUNCTION(BlueprintCallable)
	void LoadGame(FString SaveSlot, ASoulsCharacter* SoulsCharacter);
};
