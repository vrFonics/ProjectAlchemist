#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectAlbatross/ActorComponents/PlayerComponents/AC_HealthComponent.h"
#include "ProjectAlbatross/ActorComponents/PlayerComponents/AC_LockOnComponent.h"
#include "ProjectAlbatross/ActorComponents/PlayerComponents/AC_SoulsCameraComponent.h"
#include "ProjectAlbatross/ActorComponents/PlayerComponents/AC_WeaponHolder.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "ProjectAlbatross/ActorComponents/PlayerComponents/AC_StaminaComponent.h"
#include "SoulsCharacter.generated.h"

UENUM()
enum ESoulsInput
{
	ESI_None,
	ESI_LightAttack,
	ESI_HeavyAttack,
	ESI_Blink,
	ESI_QuickShot,
	ESI_SpecialRangedShot
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
	Cinematic UMETA(DisplayName = "Cinematic"),
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
struct FInventoryItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FInventoryItem()
		: ItemDisplayName("New Item")
	, ItemDescription("This is a new item.")
	, ItemImage(nullptr)
	, MaxAmount(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmount;
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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyPlayerOfBlinkStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyPlayerOfBlinkFinished();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyPlayerOfHealthHalved();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyPlayerOfQuickshot();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyPlayerOfSamplesAdded(int SamplesAdded);

	//Returns current camera state
	UFUNCTION(BlueprintCallable)
	ECameraState GetCameraState();

	//Updates the soulscharacterstate
	UFUNCTION()
	void UpdateSoulsCharacterState(ESoulsCharacterState NewState);

	UFUNCTION(BlueprintCallable)
	void ToggleMenu(bool bAddMenuContext, bool bCanPlayerMove);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement and camera rotation input
	UFUNCTION()
	void MovePlayer(const FInputActionValue& Value);
	/*
	UFUNCTION()
	void MoveX(float AxisValue);
	*/
	UFUNCTION()
	void RotateCamera(const FInputActionValue& Value);
	/*
	UFUNCTION()
	void RotateCameraX(float AxisValue);
	*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D IntendedDirection;

	//Attempt to lock on, if already locked on will cancel lock-on
	UFUNCTION()
	void TryLockOn();
	UFUNCTION()
	void TrySwitchLockOnTarget(const FInputActionValue& Value);

	//Blink functions
	UFUNCTION()
	void Blink();
	UFUNCTION()
	void FinishBlink();
	UFUNCTION()
	FVector GetBlinkDirection();

	//Updates SpringArm Settings
	UFUNCTION()
	void UpdateSpringArmSettings(FSpringArmSettings NewSettings, ECameraState NewState);

	//locked cameraRotation
	UFUNCTION()
	void DoLockedCameraRotation(float DeltaTime);

	//Attack functions
	UFUNCTION()
	void LightAttack();
	UFUNCTION()
	void HeavyAttack();

	void QuickShot();
	void SpecialRangedShot();
	void EngageRangedMode();
	void DisengageRangedMode();

    UFUNCTION(BlueprintCallable)
	void AddInfection(int Amount);

	//UPROPERTY()
	//float CameraPitchAngle;

	//Target rotation of springarm using current rotation input values and spring arm settings
	UPROPERTY()
	FRotator TargetSpringArmRotation;

	UPROPERTY()
	FRotator PreviousSpringArmRotation;

	//Calculated direction of blink
	UPROPERTY()
	FVector BlinkDirection;
	UPROPERTY()
	FVector BlinkTargetLocation;
	UPROPERTY()
	FVector PreviousLocation;
	UPROPERTY()
	float TimeSinceBlinkStarted;
	//Timer handle to restore movement ability after blink is over
	UPROPERTY()
	FTimerHandle BlinkTimer;

	//Current camera state
	UPROPERTY()
	TEnumAsByte<ECameraState> CameraState;
	//Previous state of character
	UPROPERTY()
	TEnumAsByte<ESoulsCharacterState> PreviousCharacterState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlinkDuration;
	//Has the player already switched targets and input hasn't been reset below threshold yet?
	UPROPERTY()
	bool bJustSwitchedTarget;
#pragma region /** Input */
	//Input
	bool bInputQueueable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* MovingMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* LockedOnMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* RangedMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* MenuMappingContext;

	UPROPERTY()
	TMap<class UInputMappingContext*, int> PreviousMappingContexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* PlayerMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* PlayerLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* BlinkAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* HealAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* LightAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* LockOnAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* SwitchLockOnTargetAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* EngageRangedModeAction;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* DisengageRangedModeAction;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* QuickShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* SpecialRangedShotAction;
#pragma endregion 

    bool bIsInRanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDamageable;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESoulsCharacterState> SoulsCharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_HealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_ElixirComponent* ElixirComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_LockOnComponent* LockOnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_StatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_SoulsCameraComponent* SoulsCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_WeaponHolder* WeaponHolderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_StaminaComponent* StaminaComponent;
	
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
