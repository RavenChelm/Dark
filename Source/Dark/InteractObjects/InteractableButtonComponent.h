// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IInteractable.h"
#include "InteractableButtonComponent.generated.h"


class UTimelineComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UInteractableButtonComponent : public UActorComponent, public IInteractable
{
	GENERATED_BODY()

public:
	UInteractableButtonComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Interact_Implementation(const AActor* Other) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TArray<AActor*> ControlledActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	bool bBlockAfterInteraction;

private:
	bool bBlock = false;
};
