// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HandsControllerComponent.generated.h"

class IWeapon;
class IThrowable;
class IInteractable;
class ADarkCharacter;

UENUM(BlueprintType)
enum class EHandsMode : uint8
{
	Hands_Empty               UMETA(DisplayName = "Empty"),
	Hands_Item				  UMETA(DisplayName = "Item"),
	// Hands_Medium_Item         UMETA(DisplayName = "Medium_Item"),
	// Hands_Heavy_Item          UMETA(DisplayName = "Heavy_Item"),
	// Hands_Ranged              UMETA(DisplayName = "Ranged"),
	// Hands_Melee               UMETA(DisplayName = "Melee"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARK_API UHandsControllerComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY()
	ADarkCharacter* Character;
	UPROPERTY()
	AActor* InteractActor;
	UPROPERTY()
	UObject* InteractItem;
	UPROPERTY()
	UObject* PossiblyItem;

public:	
	UHandsControllerComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwitchInteractionPrompt(bool Switcher);
	
	UFUNCTION()
	void StartInteract();
	UFUNCTION()
	void StartLongInteract();
	void CancelLongInteract();
	UFUNCTION()
	void StartAttack();

	void SetInteractItem(UObject* Object) {InteractItem = Object;}
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 500.f;
	UPROPERTY(EditAnywhere, Category = "Throw")
	int Force = 1000;
	EHandsMode CurrentMode = EHandsMode::Hands_Empty;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<IWeapon> ActiveWeapon;
};