// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Camera/CameraComponent.h"
#include "MovementSystem/CustomCharacterMovementComponent.h"
#include "DarkCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
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

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	/** Custom Movement Component with advanced movement*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

	

public:
	ADarkCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCrouch);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunStop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlideStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlideStop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClimbLedgeStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClimbLedgeStop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClimbRopeStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClimbRopeStop);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpStop);


	UPROPERTY(BlueprintAssignable)
	FOnSprintStart OnSprintStart;
	UPROPERTY(BlueprintAssignable)
	FOnSprintStop OnSprintStop;

	UPROPERTY(BlueprintAssignable)
	FOnCrouch OnCrouch;

	UPROPERTY(BlueprintAssignable)
	FOnWallRunStart OnWallRunStart;
	UPROPERTY(BlueprintAssignable)
	FOnWallRunStop OnWallRunStop;

	UPROPERTY(BlueprintAssignable)
	FOnSlideStart OnSlideStart;
	UPROPERTY(BlueprintAssignable)
	FOnSlideStop OnSlideStop;

	UPROPERTY(BlueprintAssignable)
	FOnClimbLedgeStart OnClimbLedgeStart;
	UPROPERTY(BlueprintAssignable)
	FOnClimbLedgeStop OnClimbLedgeStop;

	UPROPERTY(BlueprintAssignable)
	FOnClimbRopeStart OnClimbRopeStart;
	UPROPERTY(BlueprintAssignable)
	FOnClimbRopeStop OnClimbRopeStop;
	
	UPROPERTY(BlueprintAssignable)
	FOnJumpStart OnJumpStart;
	UPROPERTY(BlueprintAssignable)
	FOnJumpStop OnJumpStop;
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartSprint();
	void StopSprint();
	void StartCrouch();
	void StopCrouch();
	virtual void Jump() override;
	virtual void StopJumping() override;


	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


//Get methods
public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UCustomCharacterMovementComponent* GetCustomMovement() const { return CustomCharacterMovementComponent; };
	//TODO: Get Capsule
};

