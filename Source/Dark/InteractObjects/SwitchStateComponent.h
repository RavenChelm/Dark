// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "ILongInteractable.h"
#include "Components/ActorComponent.h"
#include "SwitchStateComponent.generated.h"

UENUM(BlueprintType)
enum class ESwitchType : uint8
{
	Toggle,     // Рычаг с двумя состояниями
	Momentary,  // Кнопка, возвращающаяся в исходное положение
	Continuous  // Вентиль
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARK_API USwitchStateComponent : public UActorComponent, public IInteractable, public ILongInteractable
{
	GENERATED_BODY()

public:
	USwitchStateComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Interact_Implementation(const AActor* Other) override;

	virtual void StartLongInteract_Implementation(const AActor* Other) override;
	virtual void TickLongInteract_Implementation(float DeltaTime) override;
	virtual void CompleteLongInteract_Implementation() override;
	virtual void CancelLongInteract_Implementation() override;

	UPROPERTY(EditDefaultsOnly, Category = "Switch")
	USceneComponent* SubObjectInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	ESwitchType SwitchType = ESwitchType::Continuous;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TArray<AActor*> ControlledActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	float ProgressChangeSpeed = 0.1f;
	
private:
	float Progress = 0.0f;
	bool bProcessIsActive = false;
};
