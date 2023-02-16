// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectAlbatross/ActorComponents/EnemyComponents/AC_EnemyAttackHolder.h"
#include "ProjectAlbatross/Actors/PlayerActors/SoulsCharacter.h"
#include "EnemyBase.generated.h"

UCLASS()
class PROJECTALBATROSS_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UFUNCTION(BlueprintCallable)
	int AddHealth(int HealthToAdd);

	UFUNCTION(BlueprintCallable)
	int RemoveHealth(int HealthToRemove);

	UFUNCTION(BlueprintCallable)
	int GetCurrentHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAttackOfNewDamageEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAttackOfDamageTick();

	UFUNCTION(BlueprintImplementableEvent)
	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	int CurrentHealth;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_EnemyAttackHolder* EnemyAttackHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EStaggerableState> StaggerableState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyLevel;
};
