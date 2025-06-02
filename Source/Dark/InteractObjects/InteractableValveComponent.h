// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ILongInteractableSync.h"
#include "InteractableValveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UInteractableValveComponent : public UActorComponent, public ILongInteractableSync
{
	GENERATED_BODY()

protected:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TArray<AActor*> ControlledActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	bool bBlockAfterInteraction;

	UInteractableValveComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void StartLongInteractSync_Implementation(const AActor* Other) override;
	virtual void TickLongInteractSync_Implementation(float Progress) override;
	virtual void CompleteLongInteractSync_Implementation() override;
	virtual void CancelLongInteractSync_Implementation() override;
};
