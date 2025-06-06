// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlledGateComponent.h"


UControlledGateComponent::UControlledGateComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UControlledGateComponent::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetOwner()->GetActorLocation();
    TargetLocation = StartLocation + MoveOffset;
    if (bIsOpenPreGame)
    {
        GetOwner()->SetActorLocation(TargetLocation);
        MoveDirection = 1;
    }
}

void UControlledGateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bIsMoving)
    {
        MoveProgress += MoveDirection * CurrentSpeed * DeltaTime;
        MoveProgress = FMath::Clamp(MoveProgress, 0.f, 1.f);

        const FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, MoveProgress);
        GetOwner()->SetActorLocation(NewLocation);
    
        if ((MoveDirection > 0 && MoveProgress >= 1.f) || 
            (MoveDirection < 0 && MoveProgress <= 0.f))
        {
            bIsMoving = false;
        }
    }
}

void UControlledGateComponent::Toggle_Implementation(const AActor* Instigator)
{
    MoveDirection *= -1;
    CurrentSpeed = MoveDirection > 0.f ? OpenSpeed : CloseSpeed;
    bIsMoving = true;
}

void UControlledGateComponent::SetState_Implementation(const AActor* Instigator, bool SwitchState)
{
    MoveDirection = SwitchState ? 1.f : -1.f;
    CurrentSpeed = MoveDirection > 0.f ? OpenSpeed : CloseSpeed;
    bIsMoving = true;
}
