// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ISwitchControllable.h"
#include "ControlledGateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UControlledGateComponent : public UActorComponent, public ISwitchControllable
{
	GENERATED_BODY()
public:
	UControlledGateComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Gate")
	FVector MoveOffset; 
    
	UPROPERTY(EditAnywhere, Category = "Gate", meta = (ClampMin = 0.1))
	float OpenSpeed = 1.0f; 
    
	UPROPERTY(EditAnywhere, Category = "Gate", meta = (ClampMin = 0.1))
	float CloseSpeed = 3.0f; 

	UPROPERTY(EditAnywhere, Category = "Gate")
	bool bIsOpenPreGame = false;
public:
	virtual void Toggle_Implementation(const AActor* Instigator) override;
	virtual void SetState_Implementation(const AActor* Instigator, bool SwitchState) override;

private:
	FVector StartLocation;
	FVector TargetLocation;
	float MoveProgress = 0.f;
	float MoveDirection = -1.f; // Потому что изначально ворота закрыты, при переключение направление изменится на противоположное
	float CurrentSpeed = 0.f;
	bool bIsMoving = false;

};
