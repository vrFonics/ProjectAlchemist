// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "AC_StatsComponent.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	
	FPlayerStat()
	: StatID(0)
	, StatLevel(0)
	{}
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StatID;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString StatDisplayName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 StatLevel;
};

USTRUCT(BlueprintType)
struct FStatLevelCost : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	
	FStatLevelCost()
	: StatLevel(0)
	, StatCost(0)
	{}
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StatLevel;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StatCost;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_StatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_StatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Adds Plague Samples and returns the new amount. */
	UFUNCTION(BlueprintCallable)
	int AddPlagueSamples(int Amount);

	/** Removes Plague Samples and returns the new amount. Protects against going below 0, but does not contain protection against removing more Plague Samples than the player currently has. If used for Spending make sure you check total Plague Sample amount before removal. */
	UFUNCTION(BlueprintCallable)
	int RemovePlagueSamples(int Amount);

	/** Returns the amount of Plague Samples. */
	UFUNCTION(BlueprintCallable)
	int GetPlagueSamples();

	/** Returns the current Player Level. */
	UFUNCTION(BlueprintCallable)
	int GetPlayerLevel();

	/** Returns the Stat data (Stat ID, Display Name, Stat Level) for a given Stat ID. */
	UFUNCTION(BlueprintCallable)
	FPlayerStat GetStatByID(int StatID);

	/** Returns the total Plague Sample cost for a given number of Stat Upgrades. */
	UFUNCTION(BlueprintCallable)
	int GetCostToUpgrade(int UpgradeTimes);

	/** Upgrades a Stat via StatID by 1 Level. Contains protection against leveling up if individual cost is too high, but ensure that the total Upgrade Cost is less than or equal to the Player's Plague Samples if Upgrading multiple Stats via a single action. */
	UFUNCTION(BlueprintCallable)
	bool UpgradeStat(int StatID);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<FPlayerStat> Stats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	const UDataTable* StatsDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	const UDataTable* StatsCurveDataTable;

	UPROPERTY()
	int PlagueSamples;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxPlagueSamples;

	UPROPERTY()
	int PlayerLevel;
};
