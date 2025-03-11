// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

class UClimbRopeState;
class UWallRunState;
class USlideState;
class UCrouchState;
class UJumpState;
class UClimbLadgeState;
class UWalkState;
class USprintState;
class UMovementState;
class ADarkCharacter;

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	MOVE_Previous            UMETA(DisplayName = "Previous"),
	MOVE_Sprint              UMETA(DisplayName = "Sprint"),
	MOVE_Walk                UMETA(DisplayName = "Walk"),
	MOVE_Slide               UMETA(DisplayName = "Slide"),
	MOVE_Crouch              UMETA(DisplayName = "Crouch"),
	MOVE_Jump                UMETA(DisplayName = "Jump"),
	MOVE_WallRun			 UMETA(DisplayName = "WallRun"),
	MOVE_ClimbLadge          UMETA(DisplayName = "Climb Ladge"),
	MOVE_ClimbRope           UMETA(DisplayName = "Climb Rope"),
};
enum class WallDirection { None, Left, Right };

/**
 * 
 */
UCLASS()
class DARK_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	UPROPERTY()
	TObjectPtr<UMovementState> CurrentState;
	UPROPERTY()
	TObjectPtr<UMovementState> PreviousState;
	UPROPERTY()
	TObjectPtr<USprintState> SprintState;
	UPROPERTY()
	TObjectPtr<UWalkState> WalkState;
	UPROPERTY()
	TObjectPtr<UJumpState> JumpState;
	UPROPERTY()
	TObjectPtr<UClimbLadgeState> ClimbLadgeState;
	UPROPERTY()
	TObjectPtr<UCrouchState> CrouchState;
	UPROPERTY()
	TObjectPtr<USlideState> SlideState;
	UPROPERTY()
	TObjectPtr<UWallRunState> WallRunState;
	UPROPERTY()
	TObjectPtr<UClimbRopeState> ClimbRopeState;
	
	UPROPERTY()
	ADarkCharacter* Character;
	UPROPERTY()
	UCapsuleComponent* Capsule;
	
public:
	//Basic function
	UCustomCharacterMovementComponent();
	UFUNCTION()
	virtual void BeginPlay() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void ChangeState(UMovementState* NewState);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Handled Input
	UFUNCTION()
	void HandleSprintStart();
	UFUNCTION()
	void HandleSprintStop();
	UFUNCTION()
	void HandleJumpStart();
	UFUNCTION()
	void HandleJumpStop();
	UFUNCTION()
	void HandleCrouch();
	UFUNCTION()
	void HandleSprint();
	
	//Helping Function
	bool CanClimb();
	bool CanStande();
	bool CanWallRun(WallDirection& wallDir);
	bool MoveCharacter(const FVector& Target, float DeltaTime) ;
	FVector CalculateFloorInfluence(FVector FloorNormal);
	bool VectorsEqual(const FVector& Vector1, const FVector& Vector2, float Tolerance);

	//Timeline Function
	UFUNCTION()
	void CrouchTimeLineProgress(float Value);
	UFUNCTION()
	void SlideTimeLineProgress(float Value);
	UFUNCTION()
	void CameraHorizontalAngleTimeLineProgress(float Value);
	UFUNCTION()
	void ClimbTimeLineProgress(float Value) const;

	//Getters
	UMovementState* GetState(const ECustomMovementMode Mode) const;
	UCapsuleComponent* GetCapsule() const { return Capsule; };
	ADarkCharacter* GetCharacter() const { return Character; };

	//Timeline Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* CrouchTimelineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* SlideTimelineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* CameraTimelineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* ClimbTimelineComponent;

	//GeneralMovementSettings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Movement")
	float GeneralAirConroll = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Movement")
	float GeneralGravityScale = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Movement")
	float GeneralJumpHeight = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Movement")
	float GeneralGroundFriction = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Movement")
	float GeneralBrakingDeceleration = 2048.f;
	
	//SprintState Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run")
	float SprintMaxSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run")
	float SprintMaxAcceleration = 1000.f;
	//WalkState Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float WalkMaxSpeed = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float WalkMaxAcceleration = 500.f;
	
	//Crouch Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float CrouchMaxSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float CrouchMaxAcceleration = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float BaseEyeHeight;
	float StandingCapsuleHalfHeight = 90 ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float SitingCapsuleHalfHeight = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float StandingCameraZOffeset = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float SitingCameraZOffeset = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	UCurveFloat* CrouchCurve;

	//SlideState Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideMaxSpeed = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideMaxAcceleration = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float GroundFrictionSlide = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float BrakingDecelerationSlide = 2048.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	double thresholdVelosity = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	UCurveFloat* SlideCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	UCurveFloat* CameraCurve;
	
	//Climb Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
	float ClimbSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
	float JumpFromWallSpeed_XY = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
	float JumpFromWallSpeed_Z = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb")
	UCurveFloat* ClimbCurve;

	//JumpState Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpMaxSpeed = 1500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Jump")
	float JumpMaxAcceleration = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float WallDistance = 40.f;

	//WallRun Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float FVup = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float FVforward = 1000.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float MaxTimeRun = 1.f;
	float CurrentTimeRun = 0.f;
	WallDirection wallDirection = WallDirection::None;
	WallDirection prevWallDirection = WallDirection::None;

};
