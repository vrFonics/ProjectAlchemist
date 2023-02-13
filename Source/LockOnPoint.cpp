// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnPoint.h"

// Sets default values
ALockOnPoint::ALockOnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TestArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TestArrow"));
	TestArrow->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TestArrow->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ALockOnPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALockOnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

