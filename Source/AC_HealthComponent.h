// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_HealthComponent();

	UFUNCTION(BlueprintCallable)
	int GetCurrentHealthAmount();

	UFUNCTION(BlueprintCallable)
	int RemoveHealth(int Amount);

	UFUNCTION(BlueprintCallable)
	int AddHealth(int Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	int HealthAmount;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealthAmount;

	DECLARE_DELEGATE(FKillDelegate)

	FKillDelegate KillDelegate; 
};
