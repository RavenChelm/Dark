// Fill out your copyright notice in the Description page of Project Settings.


#include "IgnitableObjectComponent.h"

#include "EElementalType.h"
#include "Particles/ParticleSystemComponent.h" 

UIgnitableObjectComponent::UIgnitableObjectComponent(): FlameParticleComponent(nullptr)
{
}

void UIgnitableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetCollisionProfileName(FName("InteractableObject"));
}

void UIgnitableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UIgnitableObjectComponent::ReactToElement_Implementation(EElementalType& ElementType, AActor* Instigator,
	const FHitResult& Hit)
{
	switch (ElementType)
	{
	case EElementalType::None:
		if (bIsBurn)
		{
			ElementType = EElementalType::Fire;
		}
		return true;
	case EElementalType::Fire:
		Ignition();
		return true;
	case EElementalType::Water:
		Extinguish(); //Создать облако пара
		SpawnSteamCloud(Instigator, Hit);
		return true;
	case EElementalType::Electric:
		//Если через огонь пролетит электрическая стрела, то пламя ионизируется(меняет цвет) и стрела взрывается, отталкивая врагов, наносит урон и тушит пламя
		if (bIsBurn)
		{
			Extinguish();
			//Explosion();
		}
		return true;
	default:
		return false;
	}
}


void UIgnitableObjectComponent::Ignition()
{
	if (!bIsBurn && FlameParticleComponent)
	{
		FlameParticleComponent->Activate();
		bIsBurn = true;
	}
}

void UIgnitableObjectComponent::Extinguish()
{
	if (bIsBurn && FlameParticleComponent)
	{
		FlameParticleComponent->Deactivate();
		bIsBurn = false;
	}
}

void UIgnitableObjectComponent::SpawnSteamCloud(AActor* Instigator, const FHitResult& Hit)
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

