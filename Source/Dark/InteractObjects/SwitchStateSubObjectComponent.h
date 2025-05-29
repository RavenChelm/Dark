// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISwitchControllable.h"
#include "Components/ActorComponent.h"
#include "SwitchStateSubObjectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARK_API USwitchStateSubObjectComponent : public UActorComponent, public ISwitchControllable
{
	GENERATED_BODY()

public:    
	USwitchStateSubObjectComponent();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
	// ISwitchControllableInterface implementation
	virtual void UpdateState_Implementation(float Progress) override;
	virtual void Toggle_Implementation(AActor* Instigator) override;
    
	// Настройки поведения
	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bShouldMove = true;
    
	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (EditCondition = "bShouldMove"))
	FVector MoveOffset;
    
	UPROPERTY(EditAnywhere, Category = "Behavior")
	float TransitionSpeed = 5.0f;

private:
	FVector InitialLocation;
	FRotator InitialRotation;
	FVector TargetLocation;
	FRotator TargetRotation;
	bool bIsActive = false;
};

inline void USwitchStateSubObjectComponent::Toggle_Implementation(AActor* Instigator)
{
	ISwitchControllable::Toggle_Implementation(Instigator);
}
