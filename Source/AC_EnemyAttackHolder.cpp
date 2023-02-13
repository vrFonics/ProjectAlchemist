// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_EnemyAttackHolder.h"

#include "MeleeEnemyAttackActor.h"

// Sets default values for this component's properties
UAC_EnemyAttackHolder::UAC_EnemyAttackHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAC_EnemyAttackHolder::EquipMeleeAttack(TSubclassOf<AMeleeEnemyAttackActor> NewMeleeAttack)
{
	UnEquipAttack();
	if (MeleeAttackDataTable == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, TEXT("MAJOR WARNING - MeleeAttackDataTable on AC_EnemyAttackHolder is not set. Ensure all DataTable variables are set correctly or all attack functionality will not work."));
		return;
	}
	CreateAttackActor(NewMeleeAttack);
	Cast<AMeleeEnemyAttackActor>(CurrentAttack)->MeleeAttackComponent->EquipMeleeAttackData(*MeleeAttackDataTable->FindRow<FEnemyMeleeAttackData>(*FString::FromInt(Cast<AMeleeEnemyAttackActor>(CurrentAttack)->MeleeAttackID), ""));
	CurrentAttack->EnemyMesh = EnemyMesh;
}

void UAC_EnemyAttackHolder::EquipRangedAttack(TSubclassOf<ARangedEnemyAttackActor> NewRangedAttack)
{
	UnEquipAttack();
	if (MeleeAttackDataTable == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, TEXT("MAJOR WARNING - RangedAttackDataTable on AC_EnemyAttackHolder is not set. Ensure all DataTable variables are set correctly or all attack functionality will not work."));
		return;
	}
	CreateAttackActor(NewRangedAttack);
}

bool UAC_EnemyAttackHolder::UnEquipAttack()
{
	if (CurrentAttack != nullptr)
	{
		GetWorld()->DestroyActor(CurrentAttack);
		return true;
	}
	return false;
}

AEnemyAttackActor* UAC_EnemyAttackHolder::GetCurrentEquippedAttack()
{
	return CurrentAttack;
}

// Called when the game starts
void UAC_EnemyAttackHolder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAC_EnemyAttackHolder::CreateAttackActor(TSubclassOf<AEnemyAttackActor> AttackType)
{
	CurrentAttack = GetWorld()->SpawnActor<AEnemyAttackActor>(AttackType, AttackSocket->GetComponentLocation(), FRotator::ZeroRotator);
	CurrentAttack->AttachToComponent(AttackSocket, FAttachmentTransformRules::KeepRelativeTransform);
	CurrentAttack->SetActorRelativeLocation(FVector::ZeroVector);
}

// Called every frame
void UAC_EnemyAttackHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

