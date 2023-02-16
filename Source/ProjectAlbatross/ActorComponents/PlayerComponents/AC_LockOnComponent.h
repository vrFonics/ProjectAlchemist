// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectAlbatross/Actors/EnemyActors/LockOnPoint.h"
#include "AC_LockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALBATROSS_API UAC_LockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_LockOnComponent();

	UFUNCTION()
	TArray<ALockOnPoint*> GetLockOnPoints();

	UFUNCTION(BlueprintCallable)
	ALockOnPoint* GetCurrentLockOnPoint();

	UFUNCTION()
	FRotator GetLockedSpringArmRotation(FVector SpringArmLocation);

	UFUNCTION()
	bool LockOn(APlayerController* Controller, FVector CameraLocation);

	UFUNCTION()
	void TryChangeTarget(APlayerController* Controller, FVector2D InputDirection, FVector CameraLocation);

	UFUNCTION(BlueprintCallable)
	void UpdateLockOnPoints(TArray<ALockOnPoint*> Points);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ALockOnPoint*> LockOnPoints;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	TMap<ALockOnPoint*, FVector2D> GetPointsOnScreen(FVector2D ViewPortSize, APlayerController* Controller, FVector CameraLocation);

	UFUNCTION()
	float GetAngleBetweenTwoVectors(FVector2D Vector1, FVector2D Vector2);
	
	UPROPERTY()
	ALockOnPoint* CurrentLockedPoint;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxLockOnDistance;
};
