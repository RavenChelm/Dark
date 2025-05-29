// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"

#include <filesystem>

#include "Dark/DarkCharacter.h"
#include "MovementState/MovementState.h"
#include "MovementState/SprintState.h"
#include "MovementState/WalkState.h"
#include "MovementState/ClimbLadgeState.h"
#include "MovementState/JumpOverObstacleState.h"
#include "MovementState/JumpState.h"
#include "MovementState/ClimbRopeState.h"
#include "MovementState/CrouchState.h"
#include "MovementState/SlideState.h"
#include "MovementState/WallRunState.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
	CrouchTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("CrouchTimelineComponent");
	SlideTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("SlideTimelineComponent");
	CameraTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("CameraTimelineComponent");
	ClimbTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("ClimbTimelineComponent");
}

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//TimeLine For Climb
	FOnTimelineFloat ClimbTimeLineProgress;
	ClimbTimeLineProgress.BindUFunction(this, FName("ClimbTimeLineProgress"));
	ClimbTimelineComponent->AddInterpFloat(ClimbCurve, ClimbTimeLineProgress);
	ClimbTimelineComponent->SetLooping(false);
	ClimbTimelineComponent->SetTimelineLength(0.3f); //Magic Number

	//Timeline for Crouch
	FOnTimelineFloat CrouchTimeLineProgress;
	CrouchTimeLineProgress.BindUFunction(this, FName("CrouchTimeLineProgress"));
	CrouchTimelineComponent->AddInterpFloat(CrouchCurve, CrouchTimeLineProgress);
	CrouchTimelineComponent->SetLooping(false);
	CrouchTimelineComponent->SetTimelineLength(0.3f); //Magic Number
	
	//TimeLine For Slide
	FOnTimelineFloat slideTimeLineProgress;
	slideTimeLineProgress.BindUFunction(this, FName("SlideTimeLineProgress"));
	SlideTimelineComponent->AddInterpFloat(SlideCurve, slideTimeLineProgress);
	SlideTimelineComponent->SetLooping(true);
	
	//TimeLine For Camera
	FOnTimelineFloat cameraTimeLineProgress;
	cameraTimeLineProgress.BindUFunction(this, FName("CameraHorizontalAngleTimeLineProgress"));
	CameraTimelineComponent->AddInterpFloat(CameraCurve, cameraTimeLineProgress);
	CameraTimelineComponent->SetLooping(false);
	CameraTimelineComponent->SetTimelineLength(0.3f);
	
	SprintState = NewObject<USprintState>(this);
	WalkState = NewObject<UWalkState>(this);

	ClimbLadgeState = NewObject<UClimbLadgeState>(this);
	ClimbRopeState = NewObject<UClimbRopeState>(this);
	JumpOverObstacleState = NewObject<UJumpOverObstacleState>(this);
	
	CrouchState = NewObject<UCrouchState>(this);
	SlideState = NewObject<USlideState>(this);

	WallRunState = NewObject<UWallRunState>(this);
	JumpState = NewObject<UJumpState>(this);

	CurrentState = WalkState;
	if (CurrentState) CurrentState->EnterState(this);
	Character = Cast<ADarkCharacter>(GetOwner());
	
	if (Character){
		Character->OnSprintStart.AddDynamic(this, &UCustomCharacterMovementComponent::HandleSprintStart);
		Character->OnSprintStop.AddDynamic(this, &UCustomCharacterMovementComponent::HandleSprintStop);
		Character->OnJumpStart.AddDynamic(this, &UCustomCharacterMovementComponent::HandleJumpStart);
		// Character->OnJumpStop.AddDynamic(this, &UCustomCharacterMovementComponent::HandleJumpStop);
		Character->OnCrouch.AddDynamic(this, &UCustomCharacterMovementComponent::HandleCrouch);
		Capsule = Character->GetComponentByClass<UCapsuleComponent>();
		BaseEyeHeight = Character->GetFirstPersonCameraComponent()->GetRelativeLocation().Z;
	}

	if (!EnableAcceleration)
	{
		MaxAcceleration = ComonAcceleration;
	}
}

void UCustomCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{

}

void UCustomCharacterMovementComponent::ChangeState(UMovementState* NewState)
{
	if (!NewState || NewState == CurrentState) return;
	PreviousState = CurrentState;
	if (CurrentState) CurrentState->ExitState(this);
	CurrentState = NewState;
	if (CurrentState) CurrentState->EnterState(this);
}

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentState) CurrentState->TickState(this, DeltaTime);
}

UMovementState* UCustomCharacterMovementComponent::GetState(const ECustomMovementMode Mode) const
{
	switch (Mode)
	{
	case ECustomMovementMode::MOVE_Previous:
		return PreviousState;
	case ECustomMovementMode::MOVE_Sprint:
		return SprintState;
	case ECustomMovementMode::MOVE_Walk:
		return WalkState;     
	case ECustomMovementMode::MOVE_Slide:
		return SlideState;     
	case ECustomMovementMode::MOVE_WallRun:
		return WallRunState;      
	case ECustomMovementMode::MOVE_ClimbLadge:
		return ClimbLadgeState;
	case ECustomMovementMode::MOVE_ClimbRope:
		return nullptr /*ClimbLadgeRope*/;
	case ECustomMovementMode::MOVE_JumpOverObstacle:
		return JumpOverObstacleState;
	case ECustomMovementMode::MOVE_Crouch:
		return CrouchState;   
	default:
		return nullptr;
	}
}

void UCustomCharacterMovementComponent::HandleSprintStart()
{
	ChangeState(SprintState);
}

void UCustomCharacterMovementComponent::HandleSprintStop()
{
	ChangeState(WalkState);
}

void UCustomCharacterMovementComponent::HandleJumpStart()
{
	ChangeState(JumpState);
}

void UCustomCharacterMovementComponent::HandleJumpStop()
{
	ChangeState(WalkState);
}

void UCustomCharacterMovementComponent::HandleCrouch()
{
	if (CurrentState == SprintState) 
	{
		ChangeState(SlideState);
		return;
	}
	if (CurrentState != CrouchState) ChangeState(CrouchState);
	else if (CanStande()) ChangeState(WalkState);
}

bool UCustomCharacterMovementComponent::CanClimb()
{
	FHitResult HitResult;
	FVector StartHead;
	FVector StartFoot = StartHead = GetOwner()->GetActorLocation();
	
	StartHead.Z += Character->GetDefaultHalfHeight()*1.5; 
	FVector EndForwardFoot = StartFoot + (Character->GetActorForwardVector() * WallDistance);
	FVector EndForwardHead = StartHead + (Character->GetActorForwardVector() * WallDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("WallTrace")), true, Character);
	bool bHitForwardFoot = Character->GetWorld()->LineTraceSingleByChannel(
		HitResult, StartFoot, EndForwardFoot, ECC_Visibility, TraceParams);
	bool bHitForwardHead = Character->GetWorld()->LineTraceSingleByChannel(
		HitResult, StartHead, EndForwardHead, ECC_Visibility, TraceParams);
	
	DrawDebugLine(Character->GetWorld(), StartFoot, EndForwardFoot, FColor::Red,
		false, -1, 0, 10);
	DrawDebugLine(Character->GetWorld(), StartHead, EndForwardHead, FColor::Yellow,
		false, -1, 0, 10);

	FVector direction = Character->GetActorLocation() - HitResult.ImpactPoint;
	direction.Normalize();
	FVector target = bHitForwardHead ? HitResult.ImpactPoint + direction * Capsule->GetUnscaledCapsuleRadius() :
					 EndForwardHead + Character->GetActorUpVector() * Character->GetDefaultHalfHeight();
	FVector targetDown = target + Character->GetActorUpVector()*(-1) * 1000 ; // TODO :: MAGIC NUMBER
	if(Character->GetWorld()->LineTraceSingleByChannel(HitResult, target, targetDown, ECC_Visibility, TraceParams))
	{
		ClimbLadgeState->Target = HitResult.ImpactPoint + Character->GetActorUpVector() * (Character->GetDefaultHalfHeight());
		DrawDebugSphere(Character->GetWorld(), ClimbLadgeState->Target, 0.5, 16, FColor::Green,
				false, 1, -1, 10);
	}

	return bHitForwardFoot && !bHitForwardHead;
}

bool UCustomCharacterMovementComponent::CanJumpOverObstacle()
{
	FHitResult HitResult;
	FVector StartHead;
	FVector StartFoot = StartHead = GetOwner()->GetActorLocation();

	StartHead.Z += Character->GetDefaultHalfHeight();
	FVector EndFoot = StartFoot + (Character->GetActorForwardVector() * MaxJumpOverObstacleDistance);
	FVector EndHead = StartHead + (Character->GetActorForwardVector() * MaxJumpOverObstacleDistance);
	FVector AfterObstacle = EndHead - (Character->GetActorUpVector() * Character->GetDefaultHalfHeight()*0.9);

	FCollisionQueryParams TraceParams(FName(TEXT("WallTrace")), true, Character);
	bool bHitFoot = Character->GetWorld()->LineTraceSingleByChannel(
		HitResult, StartFoot, EndFoot, ECC_Visibility, TraceParams);
	bool bHitHead = Character->GetWorld()->LineTraceSingleByChannel(
		HitResult, StartHead, EndHead, ECC_Visibility, TraceParams);
	bool bHitAfterObstacle = Character->GetWorld()->LineTraceSingleByChannel(
	HitResult, EndHead, AfterObstacle, ECC_Visibility, TraceParams);
	
	DrawDebugLine(Character->GetWorld(), StartFoot, EndFoot, FColor::Red,
		false, -1, 0, 10);
	DrawDebugLine(Character->GetWorld(), StartHead, EndHead, FColor::Yellow,
		false, -1, 0, 10);
	DrawDebugLine(Character->GetWorld(), EndHead, AfterObstacle, FColor::Blue,
		false, -1, 0, 10);
	
	if(bHitFoot && !bHitHead && !bHitAfterObstacle)
	{
		JumpOverObstacleState->Target = Character->GetActorLocation() + Character->GetActorForwardVector()*MaxJumpOverObstacleDistance;
		DrawDebugSphere(Character->GetWorld(), ClimbLadgeState->Target, 0.5, 16, FColor::Green,
				false, 1, -1, 10);
	}

	return bHitFoot && !bHitHead && !bHitAfterObstacle;
}

bool UCustomCharacterMovementComponent::CanStande()
{
	FHitResult HitResult;
	FVector Foot;
	FVector Head = Foot = Character->GetActorLocation();
	 
	Head.Z += Character->GetDefaultHalfHeight()/*StandingCapsuleHalfHeight*/ + 5;
	Foot.Z -= (Character->GetDefaultHalfHeight()/*StandingCapsuleHalfHeight*/ - 20);

	FCollisionQueryParams TraceParams(FName(TEXT("CanStande")), true, Character);
	bool bHitFromFootToHead = Character->GetWorld()->LineTraceSingleByChannel(HitResult, Foot, Head, ECC_Visibility, TraceParams);

	// DrawDebugLine(Character->GetWorld(), Foot, Head, FColor::Red, false, -1, 0, 10);
	
	return !bHitFromFootToHead;
}

bool UCustomCharacterMovementComponent::CanWallRun(WallDirection& wallDir)
{
	FVector Start = Character->GetActorLocation();
	FVector EndRight = Start + (Character->GetActorRightVector() * WallDistance);
	FVector EndLeft = Start + (Character->GetActorRightVector() * WallDistance * (-1));
	FCollisionQueryParams TraceParams(FName(TEXT("WallTrace")), true, Character);

	bool bHitRight = Character->GetWorld()->LineTraceSingleByChannel(WallRunState->HitRightResult, Start, EndRight, ECC_Visibility, TraceParams);
	bool bHitLeft = Character->GetWorld()->LineTraceSingleByChannel(WallRunState->HitLeftResult, Start, EndLeft, ECC_Visibility, TraceParams);

	DrawDebugLine(Character->GetWorld(), Start, EndRight, FColor::Green, false, -1, 0, 10);
	DrawDebugLine(Character->GetWorld(), Start, EndLeft, FColor::Blue, false, -1, 0, 10);

	if (bHitRight)
	{
		DrawDebugLine(Character->GetWorld(), WallRunState->HitRightResult.ImpactPoint, WallRunState->HitRightResult.ImpactPoint + WallRunState->HitRightResult.Normal, FColor::Magenta, false, -1, 0, 10);
		wallDir = WallDirection::Right;
	}
	else if (bHitLeft) 
	{
		DrawDebugLine(Character->GetWorld(), WallRunState->HitLeftResult.ImpactPoint, WallRunState->HitLeftResult.ImpactPoint+ WallRunState->HitLeftResult.Normal, FColor::Magenta, false, -1, 0, 10);
		wallDir = WallDirection::Left;
	}
	//else wallDir = WallDirection::None;

	return bHitRight || bHitLeft;
}

bool UCustomCharacterMovementComponent::MoveCharacter(const FVector& Target, float DeltaTime) 
{
	Velocity = FVector::ZeroVector;
	UpdateComponentVelocity();
	FVector CurrentPosition = Character->GetActorLocation();
	FVector NewPosition = FMath::Lerp(CurrentPosition, Target, ClimbLadgeState->LerpAlpha);
	ClimbLadgeState->LerpAlpha += DeltaTime * ClimbSpeed ;
	Character->SetActorLocation(NewPosition, false);
	if (ClimbLadgeState->LerpAlpha >= 0.98f || FVector::DistSquared(Target, CurrentPosition) <= 2.f)
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ClimbLadgeState->LerpAlpha = 0;
		return true;
	}
	return false;
}

FVector UCustomCharacterMovementComponent::CalculateFloorInfluence(FVector FloorNormal)
{
	if (VectorsEqual(FloorNormal, Character->GetActorUpVector(), 0.0001f) ){
		return FVector(0,0,0);
	}
	FVector vec =  FVector::CrossProduct(FloorNormal, (FVector::CrossProduct(FloorNormal, Character->GetActorUpVector() )));
	vec.Normalize();
	return vec;
}

bool UCustomCharacterMovementComponent::VectorsEqual(const FVector& Vector1, const FVector& Vector2, float Tolerance)
{
	if (FMath::Abs(Vector1.X - Vector2.X) <= Tolerance &&
		FMath::Abs(Vector1.Y - Vector2.Y) <= Tolerance &&
		FMath::Abs(Vector1.Z - Vector2.Z) <= Tolerance)
	{
		return true;
	}

	return false;
}



//Bring to one code style
void UCustomCharacterMovementComponent::CrouchTimeLineProgress(float Value)
{
	Character->bSimGravityDisabled = true;

	float _value = UKismetMathLibrary::Lerp(SitingCapsuleHalfHeight, StandingCapsuleHalfHeight, Value);
	auto oldHeight = Capsule->GetScaledCapsuleHalfHeight();
	Capsule->SetCapsuleHalfHeight(_value);
	auto offset = Capsule->GetScaledCapsuleHalfHeight() - oldHeight;
	auto newHeight = Capsule->GetRelativeLocation();
	newHeight.Z += offset;

	Capsule->SetRelativeLocation(newHeight);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), newHeight.Z));


	FVector newLocCam = Character->GetFirstPersonCameraComponent()->GetRelativeLocation();
	float _valueCam = FMath::GetMappedRangeValueClamped(TRange<float>(0.f, 1.f), TRange<float>(SitingCameraZOffeset, StandingCameraZOffeset), Value);
	newLocCam.Z = _valueCam;
	Character->GetFirstPersonCameraComponent()->SetRelativeLocation(newLocCam);
}

void UCustomCharacterMovementComponent::SlideTimeLineProgress(float Value)
{
	FVector Force = CalculateFloorInfluence(CurrentFloor.HitResult.Normal);
	
	AddForce(Force * 150000); //TODO: Magic Number
	if (Velocity.Length() > SlideMaxSpeed)
	{
		Velocity.Normalize();
		Velocity *= SlideMaxSpeed;
		UpdateComponentVelocity();
	}
}

void UCustomCharacterMovementComponent::CameraHorizontalAngleTimeLineProgress(float Value)
{
	const auto Controller = Character->GetController();
	const auto Rotation = Controller->GetControlRotation();
	FRotator Rotator { Rotation.Pitch,Rotation.Yaw,  (wallDirection == WallDirection::Right  || CurrentState == SlideState) ? Value : -Value };

	Controller->SetControlRotation(Rotator);
}

void UCustomCharacterMovementComponent::ClimbTimeLineProgress(const float Value) const
{
	const auto Controller = Character->GetController();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator Rotator{ Rotation.Pitch + Value, Rotation.Yaw, Rotation.Roll};

	Controller->SetControlRotation(Rotator);
}
