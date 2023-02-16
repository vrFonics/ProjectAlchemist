// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulsBlueprintFunctionLibrary.h"

#include "GameFramework/PlayerInput.h"


void USoulsBlueprintFunctionLibrary::flushInputs(const APlayerController* PlayerController)
{
	PlayerController->PlayerInput->FlushPressedKeys();
}