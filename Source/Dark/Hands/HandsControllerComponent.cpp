// Fill out your copyright notice in the Description page of Project Settings.


#include "HandsControllerComponent.h"

#include "IWeapon.h"
#include "Dark/InteractObjects/Interfaces/IInteractable.h"
#include "Dark/InteractObjects/Interfaces/IThrowable.h"
#include "Dark/InteractObjects/Interfaces/ILongInteractable.h"
#include "Dark/InteractObjects/Interfaces/ILongInteractableSync.h"
#include "Dark/Hands/CrossbowSkeletalMeshComponent.h"
#include "Dark/Hands/ElectricGauntletSkeletalMeshComponent.h"
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
		Character->OnStartCharge.AddDynamic(this, &UHandsControllerComponent::StartCharge);
		Character->OnReleaseCharge.AddDynamic(this, &UHandsControllerComponent::EndCharge);
		
		Crossbow = Character->GetCrossbowComponent();
		Gauntlet = Character->GetGauntletComponent();
		
		Crossbow->Equip_Implementation(false);
		Gauntlet->Equip_Implementation(false);
		
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
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Reset Interact Object"));
			InteractActor.Reset();
		}
		if (bCharging)
		{
			Charge(DeltaTime);
		}
		break;
	case EHandsMode::Item_Interact: 
		CurrentInteractionTime += DeltaTime;
		if (!bHit || InteractActor != HitResult.GetActor())
		{
			CancelLongSyncInteract();
			CancelInteract();
		}
		ExecuteTickLongSyncInteract(DeltaTime);
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
void UHandsControllerComponent::CancelInteract()
{
	CurrentMode = EHandsMode::Empty;
	InteractActor.Reset();
	CurrentInteractionTime = 0;
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
	else if (ActiveWeapon)
	{
		ActiveWeapon->Attack_Implementation();
	}
}

void UHandsControllerComponent::StartCharge()
{
	if (!HoldActor.IsValid() && ActiveWeapon)
	{
		ActiveWeapon->StartCharge_Implementation();
		bCharging = true;
	}
}
void UHandsControllerComponent::Charge(float DeltaTime)
{
	if (!HoldActor.IsValid() && ActiveWeapon)
	{
		ActiveWeapon->Charge_Implementation(DeltaTime);
	}
}
void UHandsControllerComponent::EndCharge()
{
	if (!HoldActor.IsValid() && ActiveWeapon)
	{
		ActiveWeapon->EndCharge_Implementation();
		bCharging = false;
	}
}

void UHandsControllerComponent::SwitchCurrenTool(int ToolNumber)
{
	switch (ToolNumber)
	{
	case 0:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Gauntlet"));
		Equip(Gauntlet);
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Sharp Arrow"));
		Equip(Crossbow, EArrowType::Sharp);
		break;
	case 2:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Fire Arrow"));
		Equip(Crossbow, EArrowType::Fire);
		break;
	case 3:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Water Arrow"));
		Equip(Crossbow, EArrowType::Water);
		break;
	case 4:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Crossbow: Electric Arrow"));
		Equip(Crossbow, EArrowType::Electric);
		break;
	case 5:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Grappling hook"));
		break;
	case 6:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Selected Smoke Bomb"));
		break;
	case 7:
		Equip(Crossbow, EArrowType::Blunt);
		break;
	}
}

void UHandsControllerComponent::Equip(const TScriptInterface<IWeapon>& Weapon, const EArrowType ArrowType)
{
	if (!ActiveWeapon)
	{
		ActiveWeapon = Weapon;
		ActiveWeapon->Equip_Implementation(true);
	}
	else if (ActiveWeapon != Weapon)
	{
		ActiveWeapon->Equip_Implementation(false);
		ActiveWeapon = Weapon;
		ActiveWeapon->Equip_Implementation(true);
	}
	if (ActiveWeapon == Crossbow)
	{
		Crossbow->SwitchArrowType(ArrowType);
	}
}

void UHandsControllerComponent::SwitchInteractionPrompt(bool Switcher)
{
}
