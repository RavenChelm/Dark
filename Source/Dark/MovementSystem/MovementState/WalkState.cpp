// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkState.h"
#include "../CustomCharacterMovementComponent.h"
void UWalkState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	// MovementComponent->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::MOVE_Walk));
	MovementComponent->MaxWalkSpeed = MovementComponent->WalkMaxSpeed;
	MovementComponent->MaxAcceleration = MovementComponent->WalkMaxAcceleration;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Walk state"));
}

void UWalkState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
}

void UWalkState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit Walk state"));
}
