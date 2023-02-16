// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackActor.h"
#include "ProjectAlbatross/ActorComponents/EnemyComponents/EnemyAttackComponents/AC_EnemyAttackComponent.h"
#include "ProjectAlbatross/ActorComponents/EnemyComponents/EnemyAttackComponents/AC_MeleeEnemyAttackComponent.h"
#include "ProjectAlbatross/Actors/WeaponActors/WeaponActor.h"
#include "MeleeEnemyAttackActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API AMeleeEnemyAttackActor : public AEnemyAttackActor
{
	GENERATED_BODY()

public:
	AMeleeEnemyAttackActor();

	virtual void Attack(EEnemyAttackRange AttackRange, EEnemyAttackSpeed AttackSpeed) override;

	UFUNCTION(BlueprintCallable)
	void DoDamageTraces();

	UFUNCTION(BlueprintCallable)
	void RefreshHitActors();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_MeleeEnemyAttackComponent* MeleeAttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MeleeAttackID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMeleeWeaponDamageTraceSetup> MeleeTraceSetups;

	UPROPERTY()
	TArray<AActor*> HitActors;
};
