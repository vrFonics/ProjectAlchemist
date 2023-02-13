// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_HealthComponent.h"
#include "AC_LockOnComponent.h"
#include "AC_SoulsCameraComponent.h"
#include "AC_WeaponHolder.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "SoulsCharacter.generated.h"

UENUM()
enum ESoulsInput
{
	ESI_None,
	ESI_LightAttack,
	ESI_HeavyAttack,
	ESI_Blink,
};

UENUM()
enum EStaggerableState
{
	StaggerableAll UMETA(DisplayName = "StaggerableAll"),
	StaggerableLight UMETA(DisplayName = "StaggerableLight"),
	StaggerableHeavy UMETA(DisplayName = "StaggerableHeavy"),
	StaggerableChargedHeavy UMETA(DisplayName = "StaggerableChargedHeavy"),
	StaggerableNone UMETA(DisplayName = "StaggerableNone"),
};

UENUM()
enum ESoulsCharacterState
{
	Moving	UMETA(DisplayName = "Moving"),
	Attacking UMETA(DisplayName = "Attacking"),
	Blinking UMETA(DisplayName = "Blinking"),
	Staggered UMETA(DisplayName = "Staggered"),
	Falling UMETA(DisplayName = "Falling"),
	Healing UMETA(DisplayName = "Healing"),
	Resting UMETA(DisplayName = "Resting"),
};

UENUM()
enum ECameraState
{
	CameraMelee	UMETA(DisplayName = "Melee"),
	CameraMeleeLockedOn UMETA(DisplayName = "MeleeLockedOn"),
	CameraRanged UMETA(DisplayName = "Ranged"),
	CameraZoomed UMETA(DisplayName = "RangedZoomed"),
	CameraCinematic UMETA(DisplayName = "Cinematic")
};

USTRUCT(BlueprintType)
struct FSpringArmSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCameraLag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsePawnControlRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInheritYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraLagSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraRotationLagSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraLagMaxDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocalSpringArmOffset;
};

UCLASS()
class PROJECTALBATROSS_API ASoulsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulsCharacter();

	//Events to call functionality on child blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyWeaponOfDamageTick();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyWeaponOfNewDamageEvent();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Die();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SuccessfullyBlinked();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SuccessfullyHeavyAttacked();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SuccessfullyLightAttacked();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SuccessfullyHipFired();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SuccessfullyZoomFired();
	UFUNCTION(BlueprintCallable)
	void UpdateCameraState(ECameraState NewState);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UsedStamina();

	//Stamina functions
	UFUNCTION(BlueprintCallable)
	float AddStamina(float StaminaToAdd);
	UFUNCTION(BlueprintCallable)
	float RemoveStamina(float StaminaToRemove);
	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina();

	//Returns current camera state
	UFUNCTION(BlueprintCallable)
	ECameraState GetCameraState();

	//Updates the soulscharacterstate
	UFUNCTION()
	void UpdateSoulsCharacterState(ESoulsCharacterState NewState);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement and camera rotation input
	UFUNCTION()
	void MoveY(float AxisValue);
	UFUNCTION()
	void MoveX(float AxisValue);
	UFUNCTION()
	void RotateCameraY(float AxisValue);
	UFUNCTION()
	void RotateCameraX(float AxisValue);

	//Attempt to lock on, if already locked on will cancel lock-on
	UFUNCTION()
	void TryLockOn();

	//Blink functions
	UFUNCTION()
	void Blink();
	UFUNCTION()
	void FinishBlink();
	UFUNCTION()
	FVector GetBlinkDirection();

	//Updates SpringArm Settings
	UFUNCTION()
	void UpdateSpringArmSettings(FSpringArmSettings NewSettings);

	//locked cameraRotation
	UFUNCTION()
	void DoLockedCameraRotation();

	//Attack functions
	UFUNCTION()
	void LightAttack();
	UFUNCTION()
	void HeavyAttack();

	//UPROPERTY()
	//float CameraPitchAngle;

	//Target rotation of springarm using current rotation input values and spring arm settings
	UPROPERTY()
	FRotator TargetSpringArmRotation;

	//Calculated direction of blink
	UPROPERTY()
	FVector BlinkDirection;
	//Timer handle to restore movement ability after blink is over
	UPROPERTY()
	FTimerHandle BlinkTimer;

	//Current camera state
	UPROPERTY()
	TEnumAsByte<ECameraState> CameraState;
	//Previous state of character
	UPROPERTY()
	TEnumAsByte<ESoulsCharacterState> PreviousCharacterState;

	//Current stamina
	UPROPERTY()
	float CurrentPlayerStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlinkDuration;
	//Has the player already switched targets and input hasn't been reset below threshold yet?
	UPROPERTY()
	bool bJustSwitchedTarget;

	bool bInputQueueable;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ProcessCurrentQueuedInput();
	
	//Current StaggerableState
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EStaggerableState> StaggerableState;

	//PLACEHOLDER controller sensitivity variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ControllerSensitivity;

	//Maximum amount of stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPlayerStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESoulsCharacterState> SoulsCharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_HealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_LockOnComponent* LockOnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_SoulsCameraComponent* SoulsCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_WeaponHolder* WeaponHolderComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
	TMap<TEnumAsByte<ECameraState>, FSpringArmSettings> SpringArmSettingsByCameraState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESoulsInput> CurrentQueuedInput;
	
	//Must be value between 0 and 1, controls how far you have to move the stick to initiate a lock on directional switch
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockOnSwitchMinimumDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* StumbleMontage;
};
