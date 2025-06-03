// Fill out your copyright notice in the Description page of Project Settings.


#include "HandsControllerComponent.h"

#include "IWeapon.h"
#include "Dark/InteractObjects/Interfaces/IInteractable.h"
#include "Dark/InteractObjects/Interfaces/IThrowable.h"
#include "Dark/InteractObjects/Interfaces/ILongInteractable.h"
#include "Dark/InteractObjects/Interfaces/ILongInteractableSync.h"
#include "Dark/Hands/CrossbowSkeletalMeshComponent.h" //need for Character->GetCrossbowComponent()
#include "Dark/Hands/SwordSkeletalMeshComponent.h"
#include "Dark/DarkCharacter.h"

UHandsControllerComponent::UHandsControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UHandsControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ADarkCharacter>(GetOwner());
	if (Character.IsValid())
	{
		Character->OnInteract.AddDynamic(this, &UHandsControllerComponent::StartInteract);
		Character->OnReleaseInteract.AddDynamic(this, &UHandsControllerComponent::ExecuteInteract);
		Character->OnAttack.AddDynamic(this, &UHandsControllerComponent::StartAttack);
		ActiveWeapon = Character->GetCrossbowComponent();
		Camera = Character->GetFirstPersonCameraComponent();
	}
}


void UHandsControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = Start + (Camera->GetComponentRotation().Vector() * InteractionDistance);
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("InteractableTrace")), true, GetOwner());
	const bool bHit = Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2, TraceParams); //Channel - Interactable

	switch (CurrentMode) {
	case EHandsMode::Empty:
		if (bHit)
		{
			InteractActor = HitResult.GetActor();
		}
		else if (InteractActor.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Reset Objects"));
			InteractActor.Reset();
		}
		break;
	case EHandsMode::Item_Interact: 
		CurrentInteractionTime += DeltaTime;
		if (!bHit || InteractActor != HitResult.GetActor())
		{
			CancelLongSyncInteract();
			CancelInteract();
		}
		ExecuteTickLongSyncInteract(CurrentInteractionTime / MaxInteractionTime * DeltaTime);
		if (CurrentInteractionTime >= MaxInteractionTime)
		{
			ExecuteInteract();
			ExecuteLongInteract();
			CompleteLongSyncInteract();
			CancelInteract();
		}
		break;
	}
}

void UHandsControllerComponent::StartInteract()
{
	if (CurrentMode == EHandsMode::Empty && InteractActor.IsValid())
	{
		CurrentMode = EHandsMode::Item_Interact;
		StartLongSyncInteraction();
	}
}
void UHandsControllerComponent::ExecuteInteract()
{
	if (InteractActor.IsValid() && !HoldActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = InteractActor->
			GetComponentsByInterface(UInteractable::StaticClass()); InteractableComponents.Num() > 0)
		{
			IInteractable::Execute_Interact(InteractableComponents[0], Character.Get());
		}
	}
	else if (HoldActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = HoldActor->
			GetComponentsByInterface(UThrowable::StaticClass()); InteractableComponents.Num() > 0)
		{
			IThrowable::Execute_Drop(InteractableComponents[0]);
			HoldActor.Reset();
		}
	}
	CancelLongSyncInteract();
	CurrentMode = EHandsMode::Empty;
	
}
void UHandsControllerComponent::ExecuteLongInteract() const
{
	if (InteractActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = InteractActor->
			GetComponentsByInterface(ULongInteractable::StaticClass()); InteractableComponents.Num() > 0)
		{
			ILongInteractable::Execute_StartLongInteract(InteractableComponents[0], Character.Get());
		}
	}
}

void UHandsControllerComponent::StartLongSyncInteraction() const
{
	if (InteractActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = InteractActor->
			GetComponentsByInterface(ULongInteractableSync::StaticClass()); InteractableComponents.Num() > 0)
		{
			ILongInteractableSync::Execute_StartLongInteractSync(InteractableComponents[0], Character.Get());
		}
	}
}
void UHandsControllerComponent::ExecuteTickLongSyncInteract(float Progress) const
{
	if (InteractActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = InteractActor->
			GetComponentsByInterface(ULongInteractableSync::StaticClass()); InteractableComponents.Num() > 0)
		{
			ILongInteractableSync::Execute_TickLongInteractSync(InteractableComponents[0], Progress);
		}
	}
}
void UHandsControllerComponent::CompleteLongSyncInteract() const
{
	if (InteractActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = InteractActor->
			GetComponentsByInterface(ULongInteractableSync::StaticClass()); InteractableComponents.Num() > 0)
		{
			ILongInteractableSync::Execute_CompleteLongInteractSync(InteractableComponents[0]);
		}
	}
}
void UHandsControllerComponent::CancelLongSyncInteract() const
{
	if (InteractActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = InteractActor->
			GetComponentsByInterface(ULongInteractableSync::StaticClass()); InteractableComponents.Num() > 0)
		{
			ILongInteractableSync::Execute_CancelLongInteractSync(InteractableComponents[0]);
		}
	}
}

void UHandsControllerComponent::CancelInteract()
{
	CurrentMode = EHandsMode::Empty;
	InteractActor.Reset();
	CurrentInteractionTime = 0;
}

void UHandsControllerComponent::StartAttack()
{
	if (HoldActor.IsValid())
	{
		if (TArray<UActorComponent*> InteractableComponents = HoldActor->
			GetComponentsByInterface(UThrowable::StaticClass()); InteractableComponents.Num() > 0)
		{
			IThrowable::Execute_Throw(InteractableComponents[0], Camera->GetComponentRotation().Vector(), Force);
			HoldActor.Reset();
		}
	}
	else
	{
		ActiveWeapon->Attack_Implementation();
	}
}

void UHandsControllerComponent::SwitchCurrenTool(int ToolNumber)
{
	UCrossbowSkeletalMeshComponent* Crossbow = Character->GetCrossbowComponent();
	switch (ToolNumber)
	{
	case 0:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Sword"));
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Sharp Arrow"));
		if (ActiveWeapon != Crossbow)
		{
			ActiveWeapon = Crossbow;
			ActiveWeapon->Equip_Implementation();
		}
		Crossbow->SwitchArrowType(EArrowType::Sharp);
		break;
	case 2:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Fire Arrow"));
		if (ActiveWeapon != Crossbow)
		{
			ActiveWeapon = Crossbow;
			ActiveWeapon->Equip_Implementation();
		}
		Crossbow->SwitchArrowType(EArrowType::Fire);
		break;
	case 3:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Water Arrow"));
		if (ActiveWeapon != Crossbow)
		{
			ActiveWeapon = Crossbow;
			ActiveWeapon->Equip_Implementation();
		}
		Crossbow->SwitchArrowType(EArrowType::Water);
		break;
	case 4:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Electric Arrow"));
		if (ActiveWeapon != Crossbow)
		{
			ActiveWeapon = Crossbow;
			ActiveWeapon->Equip_Implementation();
		}
		Crossbow->SwitchArrowType(EArrowType::Electric);
		break;
	case 5:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Grappling hook"));
		
		break;
	case 6:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Smoke Bomb"));
		break;
	case 7:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Blunt Arrow"));
		break;
	}
}


void UHandsControllerComponent::SwitchInteractionPrompt(bool Switcher)
{
}
