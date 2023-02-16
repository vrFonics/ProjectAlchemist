// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectAlbatross/ActorComponents/EnemyComponents/EnemyAttackComponents/AC_EnemyAttackComponent.h"
#include "EnemyAttackActor.generated.h"

UCLASS()
class PROJECTALBATROSS_API AEnemyAttackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyAttackActor();
	
	virtual void Attack(EEnemyAttackRange AttackRange, EEnemyAttackSpeed AttackSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SendDamage(AActor* ActorToDamage, AActor* DamageCauser, int DamageAmount);

	UFUNCTION()
	UAnimMontage* SelectAnimationByCombatSituation(TArray<FEnemyAttackAnimationData> Animations, EEnemyAttackRange AttackRange, EEnemyAttackSpeed AttackSpeed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USkeletalMeshComponent* EnemyMesh;
};
