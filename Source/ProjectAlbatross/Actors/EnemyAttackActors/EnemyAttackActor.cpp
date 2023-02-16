// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackActor.h"

// Sets default values
AEnemyAttackActor::AEnemyAttackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAttackActor::Attack(EEnemyAttackRange AttackRange, EEnemyAttackSpeed AttackSpeed)
{
	
}

// Called when the game starts or when spawned
void AEnemyAttackActor::BeginPlay()
{
	Super::BeginPlay();
	
}

UAnimMontage* AEnemyAttackActor::SelectAnimationByCombatSituation(TArray<FEnemyAttackAnimationData> Animations, EEnemyAttackRange AttackRange, EEnemyAttackSpeed AttackSpeed)
{
	TArray<UAnimMontage*> ValidMontages;

	for (FEnemyAttackAnimationData AnimationData : Animations)
	{
		if (AnimationData.AttackRange == AttackRange && AnimationData.AttackSpeed == AttackSpeed)
		{
			ValidMontages.Add(AnimationData.AnimationMontage);
		}
	}
	
	return ValidMontages[FMath::RandRange(0, ValidMontages.Num() - 1)];
}

// Called every frame
void AEnemyAttackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

