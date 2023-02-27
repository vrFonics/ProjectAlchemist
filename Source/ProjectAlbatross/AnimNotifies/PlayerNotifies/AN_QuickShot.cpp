// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectAlbatross/AnimNotifies/PlayerNotifies/AN_QuickShot.h"

#include "ProjectAlbatross/Actors/PlayerActors/SoulsCharacter.h"

void UAN_QuickShot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const ASoulsCharacter* OwningCharacter = Cast<ASoulsCharacter>(MeshComp->GetOwner());
	if (OwningCharacter != nullptr)
	{
		OwningCharacter->WeaponHolderComponent->Fire(bFireSpecial);
	}
}
