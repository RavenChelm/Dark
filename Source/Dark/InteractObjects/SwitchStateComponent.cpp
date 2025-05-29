// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchStateComponent.h"
#include "ISwitchControllable.h"

USwitchStateComponent::USwitchStateComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USwitchStateComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USwitchStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bProcessIsActive)
    {
        TickLongInteract_Implementation(DeltaTime);
    }
}

void USwitchStateComponent::Interact_Implementation(const AActor* Other)
{
    if (SwitchType == ESwitchType::Continuous) return;
}

void USwitchStateComponent::StartLongInteract_Implementation(const AActor* Other)
{
    if (SwitchType == ESwitchType::Continuous)
    {
        Progress = 0.0f;
        bProcessIsActive = true;
    }
}

void USwitchStateComponent::TickLongInteract_Implementation(float DeltaTime)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Tick Long Interact Action"));
    for (AActor* Target : ControlledActors)
    {
        if (TArray<UActorComponent*> InteractableComponents = Target->GetComponentsByInterface(USwitchControllable::StaticClass()); InteractableComponents.Num() > 0)
        {
            Progress += ProgressChangeSpeed * DeltaTime;
            ISwitchControllable::Execute_UpdateState(InteractableComponents[0], Progress);
        }
    }
}

void USwitchStateComponent::CompleteLongInteract_Implementation()
{

}

void USwitchStateComponent::CancelLongInteract_Implementation()
{
    if (SwitchType == ESwitchType::Continuous)
    {
        Progress = 0.0f;
        bProcessIsActive = false;
    }
}