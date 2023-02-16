// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectAlbatross/ActorComponents/PlayerComponents/WeaponComponents/AC_Weapon.h"
#include "ProjectAlbatross/WeaponData/WeaponDataTypes.h"
#include "WeaponActor.generated.h"

USTRUCT(BlueprintType)
struct FMeleeWeaponDamageTraceSetup
{
	GENERATED_USTRUCT_BODY()
public:
	FMeleeWeaponDamageTraceSetup()
		: TraceStart(nullptr)
		, TraceEnd(nullptr)
		, TraceRadius(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* TraceStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* TraceEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceRadius;
};

UCLASS()
class PROJECTALBATROSS_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UFUNCTION(BlueprintCallable)
	void DoMeleeTraces();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SendDamage(AActor* ActorToDamage, int DamageAmount, FSoulsDamageType DamageType);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SendAnimMontage(UAnimMontage* MontageToPlay);
	
	UFUNCTION()
	float Attack(bool IsHeavy);
	UFUNCTION()
	float Fire(bool IsZoomed);
	
	UFUNCTION(BlueprintCallable)
	void RefreshHitActorsArray();

	UFUNCTION(BlueprintCallable)
	float GetTargetAcquisition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	float PlayAnimationMontageOnCharacter(UAnimMontage* AnimMontage);

	UFUNCTION()
	void ResetCombo();

	UFUNCTION()
	FRotator GetProjectileDirection(bool IsZoomed, FRotator StartingRotation);

	bool CancelResetCombo;

	bool bCurrentIsHeavy;

	UPROPERTY()
	TArray<AActor*> HitActors;

	UPROPERTY()	
	float CurrentTargetAcquisition;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_Weapon* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMeleeWeaponDamageTraceSetup> MeleeTraces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MeleeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* RangedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CurrentCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool LightCombo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool HeavyCombo;
};
