// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideState.h"

#include "Dark/MovementSystem/CustomCharacterMovementComponent.h"

void USlideState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	
	
	MovementComponent->MaxWalkSpeed = MovementComponent->SlideMaxSpeed;
	MovementComponent->MaxAcceleration = MovementComponent->SlideMaxAcceleration;
	MovementComponent->MaxAcceleration = MovementComponent->CrouchMaxAcceleration;
	MovementComponent->GroundFriction = MovementComponent->GroundFrictionSlide;
	MovementComponent->BrakingDecelerationWalking = MovementComponent->BrakingDecelerationSlide;



	MovementComponent->CrouchTimelineComponent->PlayFromStart();
	MovementComponent->SlideTimelineComponent->PlayFromStart();
	// MovementComponent->CameraTimelineComponent->PlayFromStart();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enter Slide state"));
}

void USlideState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
	MovementComponent->Velocity -= MovementComponent->Velocity * DeltaTime;
	MovementComponent->UpdateComponentVelocity();
	
	if (MovementComponent->Velocity.Length() <= MovementComponent->thresholdVelosity/* && MovementComponent->bWantsToCrouch*/) {
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Walk));
	}
	/*if (MovementComponent->Velocity.Length() <= MovementComponent->thresholdVelosity /*&& MovementComponent->bWantsToCrouch #1#) {
		MovementComponent->EnterInNewState(MovementComponent->idleState);
	}*/
}

void USlideState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	MovementComponent->CrouchTimelineComponent->Reverse();
	MovementComponent->CameraTimelineComponent->Reverse();
	MovementComponent->SlideTimelineComponent->Stop();
	MovementComponent->GroundFriction = MovementComponent->GeneralGroundFriction;
	MovementComponent->BrakingDecelerationWalking = MovementComponent->GeneralBrakingDeceleration;
}
