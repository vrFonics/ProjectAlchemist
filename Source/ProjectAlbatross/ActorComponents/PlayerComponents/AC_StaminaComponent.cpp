// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectAlbatross/ActorComponents/PlayerComponents/AC_StaminaComponent.h"

// Sets default values for this component's properties
UAC_StaminaComponent::UAC_StaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	MaxPlayerStamina = DefaultMaxPlayerStamina;
	// ...
	
}

float UAC_StaminaComponent::AddStamina(float StaminaToAdd)
{
	CurrentPlayerStamina += StaminaToAdd;
	if (CurrentPlayerStamina > MaxPlayerStamina)
	{
		CurrentPlayerStamina = MaxPlayerStamina;
	}
	return CurrentPlayerStamina;
}

float UAC_StaminaComponent::RemoveStamina(float StaminaToRemove)
{
	CurrentPlayerStamina -= StaminaToRemove;
	if (CurrentPlayerStamina < 0)
	{
		CurrentPlayerStamina = 0;
	}
	StaminaDelegate.Execute();
	return CurrentPlayerStamina;
}

float UAC_StaminaComponent::GetCurrentStamina()
{
	return CurrentPlayerStamina;
}

// Called when the game starts
void UAC_StaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentPlayerStamina = MaxPlayerStamina;
}


// Called every frame
void UAC_StaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

