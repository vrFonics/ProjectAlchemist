// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_StaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_StaminaComponent();

	UFUNCTION(BlueprintCallable)
	float AddStamina(float StaminaToAdd);
	UFUNCTION(BlueprintCallable)
	float RemoveStamina(float StaminaToRemove);
	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Maximum amount of stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPlayerStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DefaultMaxPlayerStamina;
	
	//Current stamina
	UPROPERTY()
	float CurrentPlayerStamina;

	DECLARE_DELEGATE(UsedStaminaDelegate)
	UsedStaminaDelegate StaminaDelegate;
};
