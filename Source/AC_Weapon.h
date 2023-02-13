// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_StatsComponent.h"
#include "WeaponDataStruct.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "AC_Weapon.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_Weapon : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Weapon();

	UFUNCTION()
	void UnEquip();
	UFUNCTION()
	void Equip(FWeaponData NewWeaponData, FTransform WeaponAnchorTransform);

	UFUNCTION(BlueprintCallable)
	void ToggleMode();

	UFUNCTION(BlueprintCallable)
	bool GetMode();

	UFUNCTION(BlueprintCallable)
	FWeaponData GetWeaponData();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void AddSkeletalMesh(USkeletalMeshComponent* MeshComponent, FTransform AttachTransform, USkeletalMesh* SkeletalMesh);

	UPROPERTY()
	USkeletalMeshComponent* BaseWeaponMesh;

	UPROPERTY()
	USkeletalMeshComponent* MeleeWeaponMesh;

	UPROPERTY()
	USkeletalMeshComponent* RangedWeaponMesh;

	UPROPERTY()
	FWeaponData WeaponData;

	UPROPERTY()
	bool bMeleeMode;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_StatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponID;
};
