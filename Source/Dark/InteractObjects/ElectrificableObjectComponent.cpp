// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectrificableObjectComponent.h"

#include "EElementalType.h"


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

bool UElectrificableObjectComponent::ReactToElement_Implementation(EElementalType ElementType, AActor* Instigator,
	const FHitResult& Hit)
{
	switch (ElementType)
	{
	case EElementalType::None:
		return false;
	case EElementalType::Fire:
		return false;
	case EElementalType::Water:
		//Замыкание
		return true;
	case EElementalType::Electric:
		//Вкл/выкл
		return true;
	default:
		return false;
	}
}

