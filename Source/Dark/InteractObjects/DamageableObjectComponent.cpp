// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableObjectComponent.h"

#include "Dark/Hands/FCustomDamageType.h"


UDamageableObjectComponent::UDamageableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UDamageableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDamageableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDamageableObjectComponent::ApplyDamage_Implementation(const AActor* Instigator, const AActor* DamageCauser,
	const FCustomDamageType& DamageType)
{
	Health -= DamageType.MainDamageAmount; //TODO:: Это заглушка для абстрактных разрушаемых объектов - для конретных придётся изменить.
	if (Health <= 0)
	{
		GetOwner()->Destroy();
	}
}

