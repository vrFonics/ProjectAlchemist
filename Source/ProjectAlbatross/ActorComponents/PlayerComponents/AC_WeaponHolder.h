// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_ElixirComponent.h"
#include "Components/ActorComponent.h"
#include "ProjectAlbatross/Actors/WeaponActors/WeaponActor.h"
#include "AC_WeaponHolder.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_WeaponHolder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_WeaponHolder();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AWeaponActor> NewWeapon);

	UFUNCTION(BlueprintCallable)
	bool UnEquipCurrentWeapon();
	
	UFUNCTION(BlueprintCallable)
	float Attack(bool bIsHeavy);

	UFUNCTION(BlueprintCallable)
	bool Fire(bool bIsZoomed);

	UFUNCTION(BlueprintCallable)
	AWeaponActor* GetCurrentWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetFire();
	
	UPROPERTY()
	AWeaponActor* CurrentEquippedWeapon;

	bool bCanFire;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* WeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const UDataTable* WeaponDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_ElixirComponent* ElixirComponent;
};
