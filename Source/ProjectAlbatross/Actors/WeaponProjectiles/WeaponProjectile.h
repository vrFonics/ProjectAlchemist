// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

UCLASS()
class PROJECTALBATROSS_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ProjectileDamage;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* IgnoredActor;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoulsDamageType DamageType;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndingLocation;
};
