// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	MOVE_Sprint    UMETA(DisplayName = "Sprint"),
	MOVE_Slide     UMETA(DisplayName = "Slide"),
	MOVE_WallRun   UMETA(DisplayName = "WallRun"),
	MOVE_Climb     UMETA(DisplayName = "Climb"),
	MOVE_None      UMETA(DisplayName = "None")
};

/**
 * 
 */
UCLASS()
class DARK_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UCustomCharacterMovementComponent();

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void StartSprint();
	void StopSprint();
	void StartSlide();
	void StopSlide();
	void StartWallRun();
	void StopWallRun();
	void StartClimb();
	void StopClimb();

	bool IsCustomMovementMode(ECustomMovementMode Mode) const;

};
