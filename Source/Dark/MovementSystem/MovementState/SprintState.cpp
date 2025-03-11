// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintState.h"
#include "../CustomCharacterMovementComponent.h"
void USprintState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	// MovementComponent->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::MOVE_Sprint));
	MovementComponent->MaxWalkSpeed = MovementComponent->SprintMaxSpeed;
	MovementComponent->MaxAcceleration = MovementComponent->SprintMaxAcceleration;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Sprint state"));

}

void USprintState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{

}

void USprintState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit Sprint state"));
}
