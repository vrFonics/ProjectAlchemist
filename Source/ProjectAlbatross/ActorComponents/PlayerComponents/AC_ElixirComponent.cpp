// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_ElixirComponent.h"

// Sets default values for this component's properties
UAC_ElixirComponent::UAC_ElixirComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int UAC_ElixirComponent::GetCurrentElixir()
{
	return CurrentElixir;
}

int UAC_ElixirComponent::AddElixir(int Amount)
{
	CurrentElixir += Amount;
	if (CurrentElixir > MaxElixir)
	{
		CurrentElixir = MaxElixir;
	}
	return CurrentElixir;
}

int UAC_ElixirComponent::RemoveElixir(int Amount)
{
	CurrentElixir -= Amount;
	if (CurrentElixir <= 0)
	{
		CurrentElixir = 0;
		HealthComponent->UpdateMaxHealth((HealthComponent->MaxHealthAmount / 2));
		HealthHalved.Execute();
	}
	return CurrentElixir;
}

// Called when the game starts
void UAC_ElixirComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MaxElixir = DefaultMaxElixir;
}


// Called every frame
void UAC_ElixirComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

