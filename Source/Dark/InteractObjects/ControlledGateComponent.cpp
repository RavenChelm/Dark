// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlledGateComponent.h"


// Sets default values for this component's properties
UControlledGateComponent::UControlledGateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UControlledGateComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetOwner()->GetActorLocation() + TargetLocation;
}


// Called every frame
void UControlledGateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bItBlock)
	{
		LerpAlpha += DeltaTime * Speed;
		FVector CurrentPosition = GetOwner()->GetActorLocation();
		FVector NewPosition = FMath::Lerp(CurrentPosition, TargetLocation, LerpAlpha);
		GetOwner()->SetActorLocation(NewPosition);
		if (LerpAlpha >= 1) bItBlock = false;
	}	
}

void UControlledGateComponent::Toggle_Implementation(const AActor* Instigator)
{
	bItBlock = true;
}
void UControlledGateComponent::UpdateState_Implementation(float Progress)
{
	FVector CurrentPosition = GetOwner()->GetActorLocation();
	FVector NewPosition = FMath::Lerp(CurrentPosition, TargetLocation, Progress);
	GetOwner()->SetActorLocation(NewPosition);
}

