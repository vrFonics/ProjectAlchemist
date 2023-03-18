// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_HealthComponent.h"

// Sets default values for this component's properties
UAC_HealthComponent::UAC_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealthAmount = DefaultMaxHealthAmount;
	// ...
}

int UAC_HealthComponent::GetCurrentHealthAmount()
{
	return HealthAmount;
}


int UAC_HealthComponent::RemoveHealth(int Amount)
{
	HealthAmount -= Amount;
	if (HealthAmount <= 0)
	{
		HealthAmount = 0;
		KillDelegate.Execute();
	}
	return HealthAmount;
}

bool UAC_HealthComponent::AddHealth(int Amount)
{
	if (HealthAmount == CurrentMaxHealthAmount)
	{
		return false;
	}
	HealthAmount += Amount;
	if (HealthAmount >= CurrentMaxHealthAmount)
	{
		HealthAmount = CurrentMaxHealthAmount;
	}
	return true;
}

int UAC_HealthComponent::UpdateMaxHealth(int NewMaxHealth)
{
	CurrentMaxHealthAmount = NewMaxHealth;
	if (HealthAmount > CurrentMaxHealthAmount)
	{
		HealthAmount = CurrentMaxHealthAmount;
	}
	return HealthAmount;
}

// Called when the game starts
void UAC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentMaxHealthAmount = MaxHealthAmount;
}


// Called every frame
void UAC_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

