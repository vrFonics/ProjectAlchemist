// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_WeaponHolder.h"

#include "Engine/SkeletalMeshSocket.h"

// Sets default values for this component's properties
UAC_WeaponHolder::UAC_WeaponHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ..

	bCanFire = true;
}

void UAC_WeaponHolder::EquipWeapon(TSubclassOf<AWeaponActor> NewWeapon)
{
	if (WeaponDataTable == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, TEXT("MAJOR WARNING - WeaponDataTable is currently set to NONE. Ensure WeaponDataTable is set to DT_WeaponStats or all weapon functionality will not work."));
		return;
	}
	UnEquipCurrentWeapon();
	CurrentEquippedWeapon = GetWorld()->SpawnActor<AWeaponActor>(NewWeapon, WeaponSocket->GetComponentLocation(), FRotator::ZeroRotator);
	CurrentEquippedWeapon->AttachToComponent(WeaponSocket, FAttachmentTransformRules::KeepRelativeTransform);
	CurrentEquippedWeapon->SetActorRelativeLocation(FVector::ZeroVector);
	CurrentEquippedWeapon->WeaponComponent->Equip(*WeaponDataTable->FindRow<FWeaponData>(*FString::FromInt(CurrentEquippedWeapon->WeaponID), ""), FTransform::Identity);
	CurrentEquippedWeapon->CharacterMesh = CharacterMesh;
	CurrentEquippedWeapon->StatsComponent = StatsComponent;
}	

bool UAC_WeaponHolder::UnEquipCurrentWeapon()
{
	if (CurrentEquippedWeapon != nullptr)
	{
		GetWorld()->DestroyActor(CurrentEquippedWeapon);
		return true;
	}
	return false;
}

bool UAC_WeaponHolder::Fire(bool bIsZoomed)
{
	if (bCanFire)
	{
		const int ElixirCost = CurrentEquippedWeapon->WeaponComponent->GetWeaponData().HipFireElixirAmmoCost;
		if (ElixirComponent->GetCurrentElixir() >= ElixirCost)
		{
			ElixirComponent->RemoveElixir(ElixirCost);
		}
		else
		{
			return false; 
		}
		bCanFire = false;
		FTimerHandle FireResetHandle;
		GetWorld()->GetTimerManager().SetTimer(FireResetHandle, this, &UAC_WeaponHolder::ResetFire, /*CurrentEquippedWeapon->WeaponComponent->GetWeaponData().FireRate*/ 0.01, false);
		CurrentEquippedWeapon->Fire(bIsZoomed);
		return true;
	}
	else
	{
		return false;
	}
}

AWeaponActor* UAC_WeaponHolder::GetCurrentWeapon()
{
	return CurrentEquippedWeapon;
}

// Called when the game starts
void UAC_WeaponHolder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAC_WeaponHolder::ResetFire()
{
	bCanFire = true;
}

// Called every frame
void UAC_WeaponHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

