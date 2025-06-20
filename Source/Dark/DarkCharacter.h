// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Camera/CameraComponent.h"
#include "DarkCharacter.generated.h"

class URadialMenuControllerComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UPhysicsHandleComponent;
class UCustomCharacterMovementComponent;
class UHandsControllerComponent;
class UElectricGauntletSkeletalMeshComponent;
class UCrossbowSkeletalMeshComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADarkCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/* Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChargeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RadialMenuAction;
	
	/** Custom Movement Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

	/** Grab Point Scene  Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrabPoint;

	/** Hands Controller Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hands", meta = (AllowPrivateAccess = "true"))
	UHandsControllerComponent* HandsController;

	/** Crossbow Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hands", meta = (AllowPrivateAccess = "true"))
	UCrossbowSkeletalMeshComponent* CrossbowComponent;

	/** Sword Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hands", meta = (AllowPrivateAccess = "true"))
	UElectricGauntletSkeletalMeshComponent* GauntletComponent;

	/** Radial Menu Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	URadialMenuControllerComponent* RadialMenuController;

public:
	ADarkCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCrouch);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseInteract);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRadialMenuActionPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRadialMenuActionReleased);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseInput);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCharge);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseCharge);
	
	UPROPERTY(BlueprintAssignable)
	FOnSprintStart OnSprintStart;
	UPROPERTY(BlueprintAssignable)
	FOnSprintStop OnSprintStop;
	UPROPERTY(BlueprintAssignable)
	FOnCrouch OnCrouch;
	UPROPERTY(BlueprintAssignable)
	FOnJumpStart OnJumpStart;
	UPROPERTY(BlueprintAssignable)
	FOnAttack OnAttack;
	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;
	UPROPERTY(BlueprintAssignable)
	FOnReleaseInteract OnReleaseInteract;
	UPROPERTY(BlueprintAssignable)
	FOnRadialMenuActionPressed OnRadialMenuActionPressed;
	UPROPERTY(BlueprintAssignable)
	FOnRadialMenuActionReleased OnRadialMenuActionReleased;
	UPROPERTY(BlueprintAssignable)
	FOnMouseInput OnMouseInput;
	UPROPERTY(BlueprintAssignable)
	FOnStartCharge OnStartCharge;
	UPROPERTY(BlueprintAssignable)
	FOnReleaseCharge OnReleaseCharge;
	
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartInteract();
	void ReleaseInteract();
	void StartSprint();
	void StopSprint();
	void StartCrouch();
	void StartAttack();
	void StartCharge();
	void ReleaseCharge();
	virtual void Jump() override;
	virtual void StopJumping() override;
	void RadialMenuPressed();
	void RadialMenuReleased();


	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


//Get methods
public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UCustomCharacterMovementComponent* GetCustomMovement() const { return CustomCharacterMovementComponent; };
	UHandsControllerComponent* GetHandsController() const { return HandsController; };
	USceneComponent* GetGrabPoint() const { return GrabPoint; };
	UCrossbowSkeletalMeshComponent* GetCrossbowComponent() const {return CrossbowComponent;};
	UElectricGauntletSkeletalMeshComponent* GetGauntletComponent() const {return GauntletComponent;};
};

