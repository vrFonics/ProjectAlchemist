// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackActor.h"
#include "MeleeEnemyAttackActor.h"
#include "RangedEnemyAttackActor.h"
#include "Components/ActorComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/DataTable.h"
#include "AC_EnemyAttackHolder.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_EnemyAttackHolder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_EnemyAttackHolder();

	UFUNCTION(BlueprintCallable)
	void EquipMeleeAttack(TSubclassOf<AMeleeEnemyAttackActor> NewMeleeAttack);
	UFUNCTION(BlueprintCallable)
	void EquipRangedAttack(TSubclassOf<ARangedEnemyAttackActor> NewRangedAttack);
	UFUNCTION(BlueprintCallable)
	bool UnEquipAttack();

	UFUNCTION(BlueprintCallable)
	AEnemyAttackActor* GetCurrentEquippedAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void CreateAttackActor(TSubclassOf<AEnemyAttackActor> AttackType);
	
	UPROPERTY()
	AEnemyAttackActor* CurrentAttack;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* MeleeAttackDataTable;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* RangedAttackDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UArrowComponent* AttackSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* EnemyMesh;
};
