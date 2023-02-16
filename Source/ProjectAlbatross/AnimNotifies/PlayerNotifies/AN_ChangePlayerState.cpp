// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ChangePlayerState.h"


void UAN_ChangePlayerState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	SoulsCharacter = Cast<ASoulsCharacter>(MeshComp->GetOwner());
	if (SoulsCharacter != nullptr)
	{
		SoulsCharacter->UpdateSoulsCharacterState(NewState);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, "Character state updated");
	}
}
