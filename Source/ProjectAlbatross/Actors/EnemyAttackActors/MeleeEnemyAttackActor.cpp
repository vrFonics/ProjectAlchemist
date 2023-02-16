// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyAttackActor.h"

AMeleeEnemyAttackActor::AMeleeEnemyAttackActor()
{
	PrimaryActorTick.bCanEverTick = true;
	MeleeAttackComponent = CreateDefaultSubobject<UAC_MeleeEnemyAttackComponent>(TEXT("Enemy Attack Component"));
}

void AMeleeEnemyAttackActor::Attack(EEnemyAttackRange AttackRange, EEnemyAttackSpeed AttackSpeed)
{
	Super::Attack(AttackRange, AttackSpeed);

	EnemyMesh->GetAnimInstance()->Montage_Play(SelectAnimationByCombatSituation(MeleeAttackComponent->MeleeAttackData.AttackAnimations, AttackRange, AttackSpeed));
}

void AMeleeEnemyAttackActor::DoDamageTraces()
{
	for (FMeleeWeaponDamageTraceSetup TraceSetup : MeleeTraceSetups)
	{
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this);
		if (EnemyMesh != nullptr)
		{
			Params.AddIgnoredActor(EnemyMesh->GetOwner());
		}
		FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceSetup.TraceRadius);
		TArray<FHitResult> OutResults;
		GetWorld()->SweepMultiByChannel(OutResults, TraceSetup.TraceStart->GetComponentLocation(), TraceSetup.TraceEnd->GetComponentLocation(), FQuat::Identity, ECollisionChannel::ECC_Pawn, Sphere, Params);
		for (FHitResult HitResult : OutResults)
		{
			if (!HitActors.Contains(HitResult.GetActor()))
			{
				SendDamage(HitResult.GetActor(), EnemyMesh->GetOwner(), MeleeAttackComponent->MeleeAttackData.Damage);
				HitActors.Add(HitResult.GetActor());
			}
		}
	}
}

void AMeleeEnemyAttackActor::RefreshHitActors()
{
	HitActors.Empty();
}


