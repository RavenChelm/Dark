// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementalWaterInteractableObjectComponent.h"
#include "EElementalType.h"

UElementalWaterInteractableObjectComponent::UElementalWaterInteractableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UElementalWaterInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UElementalWaterInteractableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UElementalWaterInteractableObjectComponent::ReactToElement_Implementation(EElementalType ElementType,
	AActor* Instigator, const FHitResult& Hit)
{
	switch (ElementType)
	{
	case EElementalType::None:
		return false;
	case EElementalType::Fire:
		SpawnSteamCloud(Instigator, Hit);
		GetOwner()->Destroy();
		return true;
	case EElementalType::Water:
		return false;
	case EElementalType::Electric:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Water now is electric"));
		return true;
	default:
		return false;
	}
	
}

void UElementalWaterInteractableObjectComponent::SpawnSteamCloud(AActor* Instigator, const FHitResult& Hit)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	const FRotator PuddleRotation = Hit.ImpactNormal.Rotation();
	const FVector SpawnLocation = Hit.ImpactPoint + 100; // TODO :: MAGIC NUMBER 
    
	if (GetWorld())
	{
		GetWorld()->SpawnActor<AActor>(SteamCloud, SpawnLocation, FQuat::Identity.Rotator(), SpawnParams);
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Spawn steam cloud was"));
}
