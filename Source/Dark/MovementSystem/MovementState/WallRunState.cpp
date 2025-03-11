// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRunState.h"

#include "Dark/DarkCharacter.h"
#include "Dark/MovementSystem/CustomCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWallRunState::EnterState(UCustomCharacterMovementComponent* MovementComponent)
{
	//Out to BluePrint
	MovementComponent->AirControl = 0.7f;
	MovementComponent->GravityScale = 0.2f;
	MovementComponent->CameraTimelineComponent->Play();
	MovementComponent->CurrentTimeRun = 0;

	MovementComponent->GetCharacter()->JumpCurrentCount = 0;
	MovementComponent->GetCharacter()->JumpMaxCount = 2;
	MovementComponent->GetCharacter()->JumpKeyHoldTime = 0;
}

void UWallRunState::TickState(UCustomCharacterMovementComponent* MovementComponent, float DeltaTime)
{
	if (MovementComponent->IsMovingOnGround()) {
		MovementComponent->prevWallDirection = WallDirection::None;
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Walk));
	}
	bool wallNear = MovementComponent->CanWallRun(MovementComponent->wallDirection);
	FVector ImpactPoint = MovementComponent->wallDirection == WallDirection::Right ? HitLeftResult.ImpactPoint : HitRightResult.ImpactPoint;

	if (MovementComponent->CurrentTimeRun <= MovementComponent->MaxTimeRun && ( wallNear || FVector::Distance(MovementComponent->GetActorLocation(), ImpactPoint) <= 40.f)) {

		MovementComponent->CurrentTimeRun += DeltaTime;

		float cosForceUp = UKismetMathLibrary::Cos(MovementComponent->CurrentTimeRun / MovementComponent->MaxTimeRun * UKismetMathLibrary::GetPI());
		MovementComponent->AddForce(MovementComponent->GetCharacter()->GetActorUpVector() * MovementComponent->FVup * (cosForceUp < 0.f ? cosForceUp * 100 : cosForceUp ) );
		
		float sinForceFwrd = UKismetMathLibrary::Sin((MovementComponent->MaxTimeRun - MovementComponent->CurrentTimeRun) / MovementComponent->MaxTimeRun * UKismetMathLibrary::GetPI());
		MovementComponent->AddForce(MovementComponent->GetCharacter()->GetActorForwardVector() * MovementComponent->FVforward * (sinForceFwrd < 0.5f ? sinForceFwrd * 0.01 : sinForceFwrd) );


		FVector velosity = MovementComponent->GetLastUpdateVelocity();
		MovementComponent->Velocity = FVector(velosity.X, velosity.Y, velosity.Z - DeltaTime * 100);
		MovementComponent->UpdateComponentVelocity();
	}
	else {
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Jump));
	}

	if (MovementComponent->GetCharacter()->JumpKeyHoldTime > 0) {

		FVector directionJump = (-1) * (MovementComponent->wallDirection == WallDirection::Right ? HitRightResult.Normal - HitRightResult.ImpactPoint : HitLeftResult.Normal - HitLeftResult.ImpactPoint);
		directionJump.Normalize();

		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green,FString::SanitizeFloat(directionJump.X) + FString::FString(" ") + FString::SanitizeFloat(directionJump.Y) + FString::FString(" ") + FString::SanitizeFloat(directionJump.Z));

		FVector Force = (
			//right/left
			(MovementComponent->wallDirection == WallDirection::Right ?
				(-1) * MovementComponent->GetCharacter()->GetActorRightVector() : MovementComponent->GetCharacter()->GetActorRightVector()) * MovementComponent->JumpFromWallSpeed_XY) +
			//Up
			 (MovementComponent->GetCharacter()->GetActorUpVector() * MovementComponent->JumpFromWallSpeed_Z) +
			//forward
			(MovementComponent->GetCharacter()->GetActorForwardVector() * MovementComponent->JumpFromWallSpeed_XY);

		MovementComponent->GetCharacter()->LaunchCharacter ( Force, true, false);
		MovementComponent->ChangeState(MovementComponent->GetState(ECustomMovementMode::MOVE_Jump));

	}
}

void UWallRunState::ExitState(UCustomCharacterMovementComponent* MovementComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exit WallRun state"));
	MovementComponent->AirControl = MovementComponent->GeneralAirConroll;
	MovementComponent->GravityScale = MovementComponent->GeneralGravityScale;
	MovementComponent->GetCharacter()->JumpMaxCount = 1;
	MovementComponent->CameraTimelineComponent->Reverse();

}
