// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_LockOnComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectAlbatross/FunctionLibraries/SoulsBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
UAC_LockOnComponent::UAC_LockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

TArray<ALockOnPoint*> UAC_LockOnComponent::GetLockOnPoints()
{
	return LockOnPoints;
}

ALockOnPoint* UAC_LockOnComponent::GetCurrentLockOnPoint()
{
	return CurrentLockedPoint;
}

FRotator UAC_LockOnComponent::GetLockedSpringArmRotation(FVector SpringArmLocation)
{
	FRotator Rotator = FRotator::ZeroRotator;
	if (CurrentLockedPoint != nullptr)
	{
		Rotator = UKismetMathLibrary::FindLookAtRotation(SpringArmLocation, CurrentLockedPoint->GetActorLocation());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, CurrentLockedPoint->GetActorLocation().ToString());
	}
	return Rotator;
}

bool UAC_LockOnComponent::LockOn(APlayerController* Controller, FVector CameraLocation)
{
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	Controller->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ViewPortSize;
	ViewPortSize.X = ScreenWidth;
	ViewPortSize.Y = ScreenHeight;
	TMap<ALockOnPoint*, FVector2D> PointsOnScreen = GetPointsOnScreen(ViewPortSize, Controller, CameraLocation);

	if (PointsOnScreen.Num() == 0)
	{
		return false;
	}
	else if (PointsOnScreen.Num() == 1)
	{
		TArray<ALockOnPoint*> Keys;
		PointsOnScreen.GetKeys(Keys);
		if (Keys.Num() == 0)
		{
			return false;
		}
		if (Keys[0] == nullptr)
		{
			return false;
		}
		CurrentLockedPoint = Keys[0];
		return true;
	}

	ALockOnPoint* CurrentClosestPoint = nullptr;
	FVector2D CurrentClosestLocation;
	
	for (TPair<ALockOnPoint*, FVector2D> Point : PointsOnScreen)
	{
		if (Point.Key == nullptr)
		{
			continue;
		}
		if (CurrentClosestPoint == nullptr)
		{
			CurrentClosestPoint = Point.Key;
			CurrentClosestLocation = Point.Value;
			continue;
		}
		else
		{
			FVector2D ScreenCenter;
			ScreenCenter.X = ScreenWidth/2;
			ScreenCenter.Y = ScreenHeight/2;
			if (FVector2D::Distance(Point.Value, ScreenCenter) < FVector2D::Distance(CurrentClosestLocation, ScreenCenter))
			{
				CurrentClosestPoint = Point.Key;
				CurrentClosestLocation = Point.Value;
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Point.Value.ToString());
			}
		}
	}
	if (CurrentClosestPoint != nullptr)
	{
		CurrentLockedPoint = CurrentClosestPoint;
		return true;
	}
	else
	{
		return false;
	}
}

void UAC_LockOnComponent::TryChangeTarget(APlayerController* Controller, FVector2D InputDirection, FVector CameraLocation)
{
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	if (Controller == nullptr)
	{
		return;
	}
	Controller->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ViewPortSize;
	ViewPortSize.X = ScreenWidth;
	ViewPortSize.Y = ScreenHeight;
	FVector2D ScreenCenter = ViewPortSize/2;
	TMap<ALockOnPoint*, FVector2D> PointsOnScreen = GetPointsOnScreen(ViewPortSize, Controller, CameraLocation);

	if (PointsOnScreen.Contains(CurrentLockedPoint))
	{
		PointsOnScreen.Remove(CurrentLockedPoint);
	}
	
	if (PointsOnScreen.Num() == 0)
	{
		return;
	}
	else if (PointsOnScreen.Num() == 1)
	{
		TArray<ALockOnPoint*> Keys;
		PointsOnScreen.GetKeys(Keys);
		if (Keys.Num() == 0)
		{
			return;
		}
		if (Keys[0] == nullptr)
		{
			return;
		}
		CurrentLockedPoint = Keys[0];
		return;
	}
	
	TPair<ALockOnPoint*, FVector2D> ClosestPoint;
	
	for (TPair<ALockOnPoint*, FVector2D> Point : PointsOnScreen)
	{
		if (Point.Key == nullptr)
		{
			return;
		}
		if (ClosestPoint.Key == nullptr)
		{
			ClosestPoint = Point;
			continue;
		}
		if (USoulsBlueprintFunctionLibrary::GetAngleBetweenTwoVectors2D(InputDirection, Point.Value - ScreenCenter) < USoulsBlueprintFunctionLibrary::GetAngleBetweenTwoVectors2D(InputDirection, ClosestPoint.Value - ScreenCenter))
		{
			if (FVector2D::Distance(Point.Value, ScreenCenter) < FVector2D::Distance(ClosestPoint.Value, ScreenCenter))
			{
				ClosestPoint = Point;
			}
		}
	}
	CurrentLockedPoint = ClosestPoint.Key;
}

void UAC_LockOnComponent::UpdateLockOnPoints(TArray<ALockOnPoint*> Points)
{
	LockOnPoints = Points;
}

// Called when the game starts
void UAC_LockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TMap<ALockOnPoint*, FVector2D> UAC_LockOnComponent::GetPointsOnScreen(FVector2D ViewPortSize, APlayerController* Controller, FVector CameraLocation)
{
	TMap<ALockOnPoint*, FVector2D> PointsOnScreen;
	if (LockOnPoints.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO LOCKON POINTS ADDED TO ARRAY");
		return PointsOnScreen;
	}
	for (auto Point : LockOnPoints)
	{
		if (Point == nullptr)
		{
			continue;
		}
		if (FVector::Distance(this->GetOwner()->GetActorLocation(), Point->GetActorLocation()) > MaxLockOnDistance)
		{
			continue;
		}
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this->GetOwner());
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, Point->GetActorLocation(), ECollisionChannel::ECC_Visibility, Params);
		if (HitResult.bBlockingHit)
		{
			if (Point->GetParentActor() == nullptr)
			{
				continue;
			}
			if (HitResult.GetActor() != Point->GetParentActor())
			{
				continue;
			}
		}
		FVector2D ScreenLocation;
		if (!Controller->ProjectWorldLocationToScreen(Point->GetActorLocation(), ScreenLocation, true))
		{
			continue;
		}
		if (((ScreenLocation.X < ViewPortSize.X) && (ScreenLocation.Y < ViewPortSize.Y)) && ((ScreenLocation.X > 0) && (ScreenLocation.Y > 0)))
		{
			PointsOnScreen.Add(Point, ScreenLocation);
		}
	}
	return PointsOnScreen;
}

// Called every frame
void UAC_LockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentLockedPoint != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, CurrentLockedPoint->GetName());
	}
	// ...
}

