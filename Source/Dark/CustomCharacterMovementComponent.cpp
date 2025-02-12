// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
	MaxWalkSpeed = 600.f; //magic number
}

void UCustomCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode) 
	{
		case (uint8)ECustomMovementMode::MOVE_Sprint:
			Velocity = UpdatedComponent->GetForwardVector() * 1200.f;
			MoveUpdatedComponent(Velocity * deltaTime, UpdatedComponent->GetComponentQuat(), true);
			break;
		case (uint8)ECustomMovementMode::MOVE_Slide:
			Velocity += FVector(100, 0, 0);
			MoveUpdatedComponent(Velocity * deltaTime, UpdatedComponent->GetComponentQuat(), true);
			break;
		case (uint8)ECustomMovementMode::MOVE_WallRun:
			Velocity += FVector(0, 300, 0);
			MoveUpdatedComponent(Velocity * deltaTime, UpdatedComponent->GetComponentQuat(), true);
			break;
		case (uint8)ECustomMovementMode::MOVE_Climb:
			Velocity = FVector(0, 0, 200);
			MoveUpdatedComponent(Velocity * deltaTime, UpdatedComponent->GetComponentQuat(), true);
			break;
		default:
			Super::PhysCustom(deltaTime, Iterations);
			break;
	}
}

void UCustomCharacterMovementComponent::StartSprint()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::MOVE_Sprint);

}

void UCustomCharacterMovementComponent::StopSprint()
{
	SetMovementMode(MOVE_Walking);
}

void UCustomCharacterMovementComponent::StartSlide()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::MOVE_Slide);
}

void UCustomCharacterMovementComponent::StopSlide()
{
	SetMovementMode(MOVE_Walking);
}

void UCustomCharacterMovementComponent::StartWallRun()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::MOVE_WallRun);
}

void UCustomCharacterMovementComponent::StopWallRun()
{
	SetMovementMode(MOVE_Walking);
}

void UCustomCharacterMovementComponent::StartClimb()
{
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::MOVE_Climb);
}

void UCustomCharacterMovementComponent::StopClimb()
{
	SetMovementMode(MOVE_Walking);
}

bool UCustomCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode Mode) const
{
	return false;
}
