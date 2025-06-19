// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectrificableObjectComponent.h"
#include "Dark/Hands/FCustomDamageType.h"
#include "Interfaces/IElectricSubObject.h"
#include "Interfaces/IInteractable.h"


UElectrificableObjectComponent::UElectrificableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UElectrificableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UElectrificableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UElectrificableObjectComponent::ReactToElement_Implementation(EElementalType& ElementType, AActor* Instigator,
	const FHitResult& Hit)
{
	if (CurrentState == EStateElectricObject::Broken) return false;
		
	switch (ElementType)
	{
	case EElementalType::None:
		return false;
	case EElementalType::Fire:
		if (CurrentState == EStateElectricObject::Wet)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Is dry now"));
			SwitchTurn(EStateElectricObject::TurnedOff);
			return true;
		}
		break;
	case EElementalType::Water:
		if (CurrentState == EStateElectricObject::TurnedOn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Short Circuit, Object Is Broken now"));
			ShortCircuit();
			return true;
		}
		if (CurrentState == EStateElectricObject::TurnedOff)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Is wet now"));
			CurrentState = EStateElectricObject::Wet;
			return true;
		}
		break;
	case EElementalType::Electric:
		if (CurrentState == EStateElectricObject::Wet)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Short Circuit, Object Is Broken now"));
            ShortCircuit();
			return true;
        }
		if (CurrentState == EStateElectricObject::TurnedOn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Is Not electric now"));
			SwitchTurn(EStateElectricObject::TurnedOff);
			return true;
		}
		if (CurrentState == EStateElectricObject::TurnedOff)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Is electric now"));
			SwitchTurn(EStateElectricObject::TurnedOn);
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

void UElectrificableObjectComponent::SwitchTurn(const EStateElectricObject& State)
{
	CurrentState = State;
	for (auto Comp : ControlledButtons)
	{
		if (TArray<UActorComponent*> Components = Comp->GetComponentsByInterface(UElectricSubObject::StaticClass()); Components.Num() > 0)
		{
			for (auto InterfaceObject : Components)
			{
				IElectricSubObject::Execute_SetBlock(InterfaceObject, State != EStateElectricObject::TurnedOn);
			}
		}
	}
}

void UElectrificableObjectComponent::ShortCircuit()
{
	CurrentState = EStateElectricObject::Broken;
	for (auto Comp : ControlledButtons)
	{
		if (TArray<UActorComponent*> Components = Comp->GetComponentsByInterface(UElectricSubObject::StaticClass()); Components.Num() > 0)
		{
			for (auto InterfaceObject : Components)
			{
				IElectricSubObject::Execute_ReactToShortCircuit(InterfaceObject, GetOwner());
			}
		}
	}
}

