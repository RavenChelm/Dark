// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableButtonComponent.h"

#include "EElementalType.h"
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

bool UInteractableButtonComponent::ReactToElement_Implementation(EElementalType ElementType, AActor* Instigator,
	const FHitResult& Hit)
{
	switch (ElementType)
	{
	case EElementalType::Fire:
		return false;
	case EElementalType::None:
		Interact_Implementation(Instigator); //Кнопка нажимается стрелой
		return true;
	case EElementalType::Water:
		return false;
	case EElementalType::Electric:
		return false;
	default:
		return false;
	}
}
