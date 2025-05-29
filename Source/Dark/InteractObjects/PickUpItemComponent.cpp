// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItemComponent.h"
#include "Dark/DarkCharacter.h"
#include "Dark/Hands/CrossbowSkeletalMeshComponent.h"

UPickUpItemComponent::UPickUpItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickUpItemComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPickUpItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPickUpItemComponent::Interact_Implementation(const AActor* Other)
{
	// IInteractable::Interact_Implementation(Other);
	const ADarkCharacter* Character = Cast<ADarkCharacter>(Other);
	switch (PickUpType)
	{
	case EPickUpType::Arrow_Blunt:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Pick Up Blunt Arrow"));
		if (Character->GetCrossbowComponent()->AddAmmunition(EArrowType::Blunt, AmmoCount))
		{
			GetOwner()->Destroy();
		}
		break;
	case EPickUpType::Arrow_Fire:
		if (Character->GetCrossbowComponent()->AddAmmunition(EArrowType::Fire, AmmoCount))
		{
			GetOwner()->Destroy();
		}
		break;
	case EPickUpType::Arrow_Water:
		if (Character->GetCrossbowComponent()->AddAmmunition(EArrowType::Water, AmmoCount))
		{
			GetOwner()->Destroy();
		}
		break;
	case EPickUpType::Food:
		//RestoreHp
		//Destroy
		break;
	case EPickUpType::Coins:
		break;
	case EPickUpType::Keys:
		break;
	case EPickUpType::Note:
		break;
	}
}

