// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_StatsComponent.h"

// Sets default values for this component's properties
UAC_StatsComponent::UAC_StatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_StatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (StatsDataTable != nullptr)
	{
		TArray<FName> RowNames = StatsDataTable->GetRowNames();
		for (auto& RowName : RowNames)
		{
			Stats.Add(*StatsDataTable->FindRow<FPlayerStat>(RowName, ""));
		}
	}

	if (StatsCurveDataTable == nullptr)
	{
		//TODO print warning string that stat cost curve datatable is not assigned in blueprint
	}
}

int UAC_StatsComponent::AddPlagueSamples(int Amount)
{
	PlagueSamples += Amount;
	if (PlagueSamples > MaxPlagueSamples)
	{
		PlagueSamples = MaxPlagueSamples;
	}
	return PlagueSamples;
}

int UAC_StatsComponent::RemovePlagueSamples(int Amount)
{
	PlagueSamples -= Amount;
	if (PlagueSamples < 0)
	{
		PlagueSamples = 0;
	}
	return PlagueSamples;
}

int UAC_StatsComponent::GetPlagueSamples()
{
	return PlagueSamples;
}

int UAC_StatsComponent::GetPlayerLevel()
{
	return PlayerLevel;
}

FPlayerStat UAC_StatsComponent::GetStatByID(int StatID)
{
	for (auto& Stat : Stats)
	{
		if (Stat.StatID == StatID)
		{
			return Stat;
		}
	}
	FPlayerStat DefaultStat;
	DefaultStat.StatID = -1;
	DefaultStat.StatDisplayName = "Stat Not Found";
	DefaultStat.StatLevel = -1;
	return DefaultStat;
}

int UAC_StatsComponent::GetCostToUpgrade(int UpgradeTimes)
{
	int TotalCost = 0;
	for (int i = 1; i < UpgradeTimes + 1; i++)
	{
		if (PlayerLevel + i <= 20)
		{
			const FStatLevelCost Cost = *StatsCurveDataTable->FindRow<FStatLevelCost>(*FString::FromInt(PlayerLevel + i), "", true);
			TotalCost += Cost.StatCost;
		}
	}
	return TotalCost;
}

bool UAC_StatsComponent::UpgradeStat(int StatID)
{
	if (PlagueSamples >= GetCostToUpgrade(PlayerLevel + 1))
	{
		RemovePlagueSamples(GetCostToUpgrade(PlayerLevel + 1));
		for (auto& Stat : Stats)
		{
			if (Stat.StatID == StatID)
			{
				Stat.StatLevel += 1;
				return true;
				PlayerLevel += 1;
			}
		}
		return false;
	}
	return false;
}

// Called every frame
void UAC_StatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



