// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbLadgeState.h"
#include "../CustomCharacterMovementComponent.h"
void UClimbLadgeState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Velocity = MovementComponent->Velocity;
	MovementComponent->ClimbTimelineComponent->PlayFromStart();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Climb Ladge state"));

}

void UClimbLadgeState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
	if (MovementComponent->MoveCharacter(Target, DeltaTime)) {
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Walk));
	}
}

void UClimbLadgeState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->Velocity = Velocity;
	MovementComponent->UpdateComponentVelocity();
	LerpAlpha = 0;
	MovementComponent->GetCapsule()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit Climb Ladge state"));

}

