// Fill out your copyright notice in the Description page of Project Settings.


#include "HandsControllerComponent.h"

#include "IWeapon.h"
#include "../InteractObjects/IInteractable.h"
#include "Dark/InteractObjects/IThrowable.h"
#include "CrossbowSkeletalMeshComponent.h"
#include "SwordSkeletalMeshComponent.h"
#include "Dark/DarkCharacter.h"
#include "Dark/InteractObjects/ILongInteractable.h"

UHandsControllerComponent::UHandsControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UHandsControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ADarkCharacter>(GetOwner());
	if (Character)
	{
		Character->OnInteract.AddDynamic(this, &UHandsControllerComponent::StartInteract);
		Character->OnAttack.AddDynamic(this, &UHandsControllerComponent::StartAttack);
		Character->OnLongInteract.AddDynamic(this, &UHandsControllerComponent::StartLongInteract);

		ActiveWeapon = Character->GetCrossbowComponent();
	}
}


void UHandsControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentMode == EHandsMode::Hands_Empty)
	{
		DrawDebugSphere(GetWorld(), Character->GetGrabPoint()->GetComponentLocation(), 10, 12, FColor::Red, false, -1);

		const FVector Start = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector End = Start + (Character->GetFirstPersonCameraComponent()->GetComponentRotation().Vector() * InteractionDistance);

		if (FHitResult HitResult; Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
		{
			if (TArray<UActorComponent*> InteractableComponents = HitResult.GetActor()->GetComponentsByInterface(UInteractable::StaticClass()); InteractableComponents.Num() > 0)
			{
				InteractActor = HitResult.GetActor();
				PossiblyItem = InteractableComponents[0];
				SwitchInteractionPrompt(true); 
			}
		}
		else
		{
			PossiblyItem = nullptr;
			SwitchInteractionPrompt(false); 
		}
	}
	else if (CurrentMode == EHandsMode::Hands_Item)
	{
		const FVector Start = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector End = Start + (Character->GetFirstPersonCameraComponent()->GetComponentRotation().Vector() * InteractionDistance);
		
		if (FHitResult HitResult; !Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility) ||
			HitResult.GetActor() != InteractActor)
		{
			CancelLongInteract();
		}
	}
}

void UHandsControllerComponent::StartInteract()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Start Interact"));

	switch (CurrentMode) {
	case EHandsMode::Hands_Empty:
		if (PossiblyItem && PossiblyItem->Implements<UInteractable>())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Do something with item"));
			IInteractable::Execute_Interact(PossiblyItem, Character);
			if (PossiblyItem->Implements<UThrowable>())
			{
				InteractItem = PossiblyItem;
				CurrentMode = EHandsMode::Hands_Item;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Do nothing"));
		}
		break;
	case EHandsMode::Hands_Item:
		if (InteractItem && InteractItem->Implements<UThrowable>())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drop Item"));
			IThrowable::Execute_Drop(InteractItem);
			InteractItem = nullptr;
			CurrentMode = EHandsMode::Hands_Empty;
		}
		break;
	}
}

void UHandsControllerComponent::StartLongInteract()
{
	if (CurrentMode == EHandsMode::Hands_Empty)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Start Long Interact Action"));
		if (PossiblyItem && PossiblyItem->Implements<ULongInteractable>())
		{
			InteractItem = PossiblyItem;
			ILongInteractable::Execute_StartLongInteract(InteractItem, Character);
			CurrentMode = EHandsMode::Hands_Item;
		}
	}
}
void UHandsControllerComponent::CancelLongInteract()
{
	if (CurrentMode == EHandsMode::Hands_Item)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Cancel Long Interact Action"));
		if (InteractItem && InteractItem->Implements<ULongInteractable>())
		{
			ILongInteractable::Execute_CancelLongInteract(InteractItem);
			InteractItem = nullptr;
			CurrentMode = EHandsMode::Hands_Empty;
		}
	}
}

void UHandsControllerComponent::StartAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Attack do throwing"));
	switch (CurrentMode) {
	case EHandsMode::Hands_Empty:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Weapon do attack"));
		ActiveWeapon->Attack_Implementation();
		break;
	case EHandsMode::Hands_Item:
		if (InteractItem && InteractItem->Implements<UThrowable>())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Throw"));
			IThrowable::Execute_Throw(InteractItem, Character->GetFirstPersonCameraComponent()->GetForwardVector(), Force);
			InteractItem = nullptr;
			CurrentMode = EHandsMode::Hands_Empty;
		}
		break;
	}
}

void UHandsControllerComponent::SwitchInteractionPrompt(bool Switcher)
{
}