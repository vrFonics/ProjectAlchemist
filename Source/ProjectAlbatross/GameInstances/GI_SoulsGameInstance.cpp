// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_SoulsGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectAlbatross/SG_SoulsSaveGame.h"

void UGI_SoulsGameInstance::SaveGame(FString SaveSlot, ASoulsCharacter* SoulsCharacter)
{
	USG_SoulsSaveGame* SaveGameInstance = Cast<USG_SoulsSaveGame>(UGameplayStatics::CreateSaveGameObject(USG_SoulsSaveGame::StaticClass()));
	SaveGameInstance->Stats = SoulsCharacter->StatsComponent->GetAllStats();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlot, 0);
}

void UGI_SoulsGameInstance::LoadGame(FString SaveSlot, ASoulsCharacter* SoulsCharacter)
{
	const USG_SoulsSaveGame* SaveGameInstance = Cast<USG_SoulsSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
	SoulsCharacter->StatsComponent->SetAllStats(SaveGameInstance->Stats);
}
