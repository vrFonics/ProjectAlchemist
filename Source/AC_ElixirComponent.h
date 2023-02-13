// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_ElixirComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_ElixirComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_ElixirComponent();

	UFUNCTION(BlueprintCallable)
	int GetCurrentElixir();

	UFUNCTION(BlueprintCallable)
	int AddElixir(int Amount);

	UFUNCTION(BlueprintCallable)
	int RemoveElixir(int Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	int CurrentElixir;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxElixir;
};
