// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyAttackHolder = CreateDefaultSubobject<UAC_EnemyAttackHolder>(TEXT("Enemy Attack Holder"));
}

int AEnemyBase::AddHealth(int HealthToAdd)
{
	CurrentHealth += HealthToAdd;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	return CurrentHealth;
}

int AEnemyBase::RemoveHealth(int HealthToRemove)
{
	CurrentHealth -= HealthToRemove;
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Die();
	}
	return CurrentHealth;
}

int AEnemyBase::GetCurrentHealth()
{
	return CurrentHealth;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

