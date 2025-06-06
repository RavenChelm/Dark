// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalSteamInteractableObjectComponent.h"
#include "EElementalType.h"

UElementalSteamInteractableObjectComponent::UElementalSteamInteractableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UElementalSteamInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UElementalSteamInteractableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UElementalSteamInteractableObjectComponent::ReactToElement_Implementation(EElementalType& ElementType,
	AActor* Instigator, const FHitResult& Hit)
{
	switch (ElementType)
	{
	case EElementalType::None:
		return false;
	case EElementalType::Fire:
		return false;
	case EElementalType::Water:
		return false;
	case EElementalType::Electric:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("steam cloud is electric now"));
		return true;
	default:
		return false;
	}
}

