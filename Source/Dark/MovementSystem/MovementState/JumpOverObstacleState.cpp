// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpOverObstacleState.h"
#include "../CustomCharacterMovementComponent.h"

void UJumpOverObstacleState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Velocity = MovementComponent->Velocity;
	Velocity.Z = 0;
	MovementComponent->ClimbTimelineComponent->PlayFromStart();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Jump Over Obstacle State"));
}

void UJumpOverObstacleState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
	if (MovementComponent->MoveCharacter(Target, DeltaTime)) {
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Sprint));
	}
}

void UJumpOverObstacleState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->Velocity = Velocity;
	MovementComponent->UpdateComponentVelocity();
	LerpAlpha = 0;
	MovementComponent->GetCapsule()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit Jump Over Obstacle State"));
}
