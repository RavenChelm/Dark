// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableButtonComponent.h"
#include "Dark/InteractObjects/Interfaces/ISwitchControllable.h"
#include "Components/TimelineComponent.h"


UInteractableButtonComponent::UInteractableButtonComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInteractableButtonComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInteractableButtonComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractableButtonComponent::Interact_Implementation(const AActor* Other)
{
	if (bBlock) return;
	
	for (const AActor* ControlledActor : ControlledActors)
	{
		if (ControlledActor)
		{
			if (TArray<UActorComponent*> Components = ControlledActor->
				GetComponentsByInterface(USwitchControllable::StaticClass()); Components.Num() > 0)
			{
				ISwitchControllable::Execute_Toggle(Components[0], Other);
				if (bBlockAfterInteraction) bBlock = true;
			}
		}
	}
}