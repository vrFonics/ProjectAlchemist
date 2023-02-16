// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_SoulsCameraComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAC_SoulsCameraComponent::UAC_SoulsCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_SoulsCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAC_SoulsCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAC_SoulsCameraComponent::DoCameraLag(float DeltaTime, float XAxisRotationValue, float YAxisRotationValue, float XAxisMovementValue, float YAxisMovementValue, UCapsuleComponent* CapsuleComponent, USpringArmComponent* SpringArmComponent, UCameraComponent* Camera)
{
	if (abs(XAxisRotationValue) <= 0.1 && abs(YAxisRotationValue) <= 0.1)
	{
		//bad memory usage. should initialize variable once and use throughout actor lifetime.
		float TargetLagSpeed;
		TargetLagSpeed = abs(UKismetMathLibrary::Dot_VectorVector(CapsuleComponent->GetForwardVector(), Camera->GetForwardVector()));
		TargetLagSpeed = pow(TargetLagSpeed, 0.5f);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(TargetLagSpeed));
		TargetLagSpeed *= 5.0f;
		TargetLagSpeed = UKismetMathLibrary::Clamp(TargetLagSpeed, 2.0f, TargetLagSpeed);
		//TargetLagSpeed = sqrt(TargetLagSpeed);
		//TargetLagSpeed += 1;
		//REPLACE ALL * 2 IN LERPS WITH "CAMERA SMOOTHNESS" VARIABLE FOR TWEAKING IN EDITOR
		SpringArmComponent->CameraLagSpeed = UKismetMathLibrary::Lerp(SpringArmComponent->CameraLagSpeed, TargetLagSpeed, DeltaTime * 2);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(SpringArmComponent->CameraLagSpeed));
		if (XAxisMovementValue != 0 || YAxisMovementValue != 0)
		{
			SpringArmComponent->AddRelativeRotation(FRotator(0, XAxisMovementValue * SpringArmComponent->CameraLagSpeed * 2.5 * DeltaTime, 0));
		}
		else
		{
			Camera->SetRelativeRotation(FQuat::Slerp(Camera->GetRelativeRotation().Quaternion(), FRotator(0, 0, 0).Quaternion(), DeltaTime * 2));
		}
	}
	else
	{
		SpringArmComponent->CameraLagSpeed = UKismetMathLibrary::Lerp(SpringArmComponent->CameraLagSpeed, 10.0f, DeltaTime * 2);
		Camera->SetRelativeRotation(FQuat::Slerp(Camera->GetRelativeRotation().Quaternion(), FRotator(0, 0, 0).Quaternion(), DeltaTime * 2));
	}
}

