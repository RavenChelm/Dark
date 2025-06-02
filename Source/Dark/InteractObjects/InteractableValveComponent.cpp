// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableValveComponent.h"

#include "Interfaces/ISwitchControllable.h"


UInteractableValveComponent::UInteractableValveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractableValveComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInteractableValveComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractableValveComponent::StartLongInteractSync_Implementation(const AActor* Other)
{
	
}

void UInteractableValveComponent::TickLongInteractSync_Implementation(float Progress)
{
	for (const AActor* ControlledActor : ControlledActors)
	{
		if (ControlledActor)
		{
			if (TArray<UActorComponent*> Components = ControlledActor->
				GetComponentsByInterface(USwitchControllable::StaticClass()); Components.Num() > 0)
			{
				ISwitchControllable::Execute_UpdateState(Components[0], Progress);
			}
		}
	}
}

void UInteractableValveComponent::CompleteLongInteractSync_Implementation()
{
}

void UInteractableValveComponent::CancelLongInteractSync_Implementation()
{
}

