// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_EnemyAttackComponent.h"

// Sets default values for this component's properties
UAC_EnemyAttackComponent::UAC_EnemyAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAC_EnemyAttackComponent::EquipMeleeAttackData(FEnemyMeleeAttackData NewData)
{
}

void UAC_EnemyAttackComponent::EquipRangedAttackData(FEnemyRangedAttackData NewData)
{
}

// Called when the game starts
void UAC_EnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAC_EnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

