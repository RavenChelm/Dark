// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchState.h"
#include "../CustomCharacterMovementComponent.h"

void UCrouchState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->MaxWalkSpeed = MovementComponent->CrouchMaxSpeed;
	if (MovementComponent->EnableAcceleration)
	{
		MovementComponent->MaxAcceleration = MovementComponent->CrouchMaxAcceleration;
	}
	MovementComponent->CrouchTimelineComponent->Play();
	OnState = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Crouch state"));
}

void UCrouchState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
}

void UCrouchState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->CrouchTimelineComponent->Reverse();
	OnState = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit Crouch state"));
}
