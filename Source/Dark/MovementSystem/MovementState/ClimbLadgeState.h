// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementState.h"
#include "ClimbLadgeState.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API UClimbLadgeState : public UMovementState
{
	GENERATED_BODY()
public:
	virtual void EnterState(UCustomCharacterMovementComponent* MovementComponent) override;
	virtual void TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime) override;
	virtual void ExitState(UCustomCharacterMovementComponent* MovementComponent) override;

	FVector Target;
	FVector Velocity;
	float LerpAlpha = 0;
};
