// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementState.h"
#include "WallRunState.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API UWallRunState : public UMovementState
{
	GENERATED_BODY()
public:
	virtual void EnterState(UCustomCharacterMovementComponent* MovementComponent) override;
	virtual void TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime) override;
	virtual void ExitState(UCustomCharacterMovementComponent* MovementComponent) override;
	FHitResult HitLeftResult;
	FHitResult HitRightResult;
};
