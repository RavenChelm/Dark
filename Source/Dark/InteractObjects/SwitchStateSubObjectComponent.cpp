#include "SwitchStateSubObjectComponent.h"

USwitchStateSubObjectComponent::USwitchStateSubObjectComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USwitchStateSubObjectComponent::BeginPlay()
{
    Super::BeginPlay();
    
    InitialLocation = GetOwner()->GetActorLocation();
    TargetLocation = InitialLocation + MoveOffset;
}

void USwitchStateSubObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USwitchStateSubObjectComponent::UpdateState_Implementation(float Progress)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Door"));

    AActor* Owner = GetOwner();
    if (!Owner) return;
    
    if (bShouldMove)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Door is moving"));
        FVector CurrentLocation = Owner->GetActorLocation();
        FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, Progress, TransitionSpeed);
        Owner->SetActorLocation(NewLocation);
    }
}