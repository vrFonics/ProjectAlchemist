// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Weapon.h"

#include "ProjectAlbatross/Actors/WeaponActors/WeaponActor.h"


// Sets default values for this component's properties
UAC_Weapon::UAC_Weapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bMeleeMode = true;
}

void UAC_Weapon::UnEquip()
{
	if (BaseWeaponMesh != nullptr)
	{
		BaseWeaponMesh->DestroyComponent();
	}
	if (MeleeWeaponMesh != nullptr)
	{
		MeleeWeaponMesh->DestroyComponent();
	}
	if (RangedWeaponMesh != nullptr)
	{
		RangedWeaponMesh->DestroyComponent();
	}
	this->DestroyComponent();
}

void UAC_Weapon::Equip(FWeaponData NewWeaponData, FTransform WeaponAnchorTransform)
{
	WeaponData = NewWeaponData;
	AWeaponActor* WeaponActor = Cast<AWeaponActor>(GetOwner());
	WeaponActor->BaseMesh->SetSkinnedAsset(WeaponData.WeaponBaseMesh);
	WeaponActor->MeleeMesh->SetSkinnedAsset(WeaponData.WeaponMeleeMesh);
	WeaponActor->RangedMesh->SetSkinnedAsset(WeaponData.WeaponRangedMesh);
}

void UAC_Weapon::ToggleMode()
{
	if (bMeleeMode == true)
	{
		bMeleeMode = false;
	}
	else
	{
		bMeleeMode = true;
	}
}

bool UAC_Weapon::GetMode()
{
	return bMeleeMode;
}

FWeaponData UAC_Weapon::GetWeaponData()
{
	return WeaponData;
}

// Called when the game starts
void UAC_Weapon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAC_Weapon::AddSkeletalMesh(USkeletalMeshComponent* MeshComponent, FTransform AttachTransform, USkeletalMesh* SkeletalMesh)
{
	MeshComponent = Cast<USkeletalMeshComponent>(GetOwner()->AddComponentByClass(USkeletalMeshComponent::StaticClass(), false, AttachTransform, true));
	GetOwner()->AddInstanceComponent(MeshComponent);
	MeshComponent->SetSkinnedAsset(SkeletalMesh);
}


// Called every frame
void UAC_Weapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

