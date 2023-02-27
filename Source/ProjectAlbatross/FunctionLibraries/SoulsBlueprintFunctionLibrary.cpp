// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulsBlueprintFunctionLibrary.h"

#include "GameFramework/PlayerInput.h"
#include "Kismet/KismetMathLibrary.h"


void USoulsBlueprintFunctionLibrary::FlushInputs(const APlayerController* PlayerController)
{
	PlayerController->PlayerInput->FlushPressedKeys();
}

float USoulsBlueprintFunctionLibrary::GetAngleBetweenTwoVectors2D(const FVector2D Vector1, const FVector2D Vector2)
{
    return UKismetMathLibrary::DegAcos(UKismetMathLibrary::DotProduct2D(Vector1.GetSafeNormal(), Vector2.GetSafeNormal()));
}
