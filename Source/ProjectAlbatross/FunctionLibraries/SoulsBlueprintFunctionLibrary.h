// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulsBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALBATROSS_API USoulsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void FlushInputs(const APlayerController* PlayerController);
	static float GetAngleBetweenTwoVectors2D(const FVector2D Vector1, const FVector2D Vector2);
};
