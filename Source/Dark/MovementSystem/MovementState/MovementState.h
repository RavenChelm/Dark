// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovementState.generated.h"

class UCustomCharacterMovementComponent;

/**
 * 
 */
UCLASS(Abstract)
class DARK_API UMovementState : public UObject
{
	GENERATED_BODY()
public:
	virtual void EnterState(UCustomCharacterMovementComponent* MovementComponent) {}
	virtual void TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime) {}
	virtual void ExitState(UCustomCharacterMovementComponent* MovementComponent) {}
};
