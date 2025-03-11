// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpState.h"
#include "../CustomCharacterMovementComponent.h"
#include "Engine/GameEngine.h"

void UJumpState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	// MovementComponent->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::MOVE_Jump));
	MovementComponent->MaxWalkSpeed = MovementComponent->JumpMaxSpeed;
	MovementComponent->MaxAcceleration = MovementComponent->JumpMaxAcceleration;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Jump state"));

}

void UJumpState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
	if (MovementComponent->CanClimb())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Check Climb Success"));
		MovementComponent->prevWallDirection = WallDirection::None;
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_ClimbLadge));
	}
	if (MovementComponent->CanWallRun(MovementComponent->wallDirection) && MovementComponent->prevWallDirection != MovementComponent->wallDirection) { // Add Condition for Run, but not just jump;
		MovementComponent->prevWallDirection = MovementComponent->wallDirection;
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_WallRun));
	}
	if (!MovementComponent->IsFalling())
	{
		if (UMovementState* PreviousState = MovementComponent->GetState(ECustomMovementMode::MOVE_Previous);
			PreviousState == MovementComponent->GetState(ECustomMovementMode::MOVE_Sprint)) MovementComponent->ChangeState(PreviousState);
		else MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Walk));
	}
}

void UJumpState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit Jump state"));
}

