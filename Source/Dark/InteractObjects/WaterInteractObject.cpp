// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterInteractObject.h"
#include "Particles/ParticleSystemComponent.h" 

void UWaterInteractObject::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystemComponent = GetOwner()->FindComponentByClass<UParticleSystemComponent>();
}

void UWaterInteractObject::ActStateUp() const
{
	if (ParticleSystemComponent) ParticleSystemComponent->Activate();
}

void UWaterInteractObject::ActStateDown() const
{
	if(ParticleSystemComponent) ParticleSystemComponent->Deactivate();
}

void UWaterInteractObject::Interact_Implementation(const AActor* Other)
{
	if (ParticleSystemComponent) ParticleSystemComponent->Activate();
}


