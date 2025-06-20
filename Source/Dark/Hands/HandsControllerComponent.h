// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrossbowSkeletalMeshComponent.h"
#include "HandsControllerComponent.generated.h"

class UElectricGauntletSkeletalMeshComponent;
class IWeapon;
class IThrowable;
class IInteractable;
class ADarkCharacter;
class UCameraComponent;

UENUM(BlueprintType)
enum class EHandsMode : uint8
{
	Empty					UMETA(DisplayName = "Empty"),
	Item_Interact		    UMETA(DisplayName = "Item")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARK_API UHandsControllerComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY()
	TWeakObjectPtr<ADarkCharacter> Character;
	UPROPERTY()
	TWeakObjectPtr<UCameraComponent> Camera; 
	UPROPERTY()
	TWeakObjectPtr<AActor> InteractActor;
	UPROPERTY()
	TWeakObjectPtr<AActor> HoldActor;
	UPROPERTY()
	TWeakObjectPtr<UObject> InteractObject;
	UPROPERTY()
	UCrossbowSkeletalMeshComponent* Crossbow;
	UPROPERTY()
	UElectricGauntletSkeletalMeshComponent* Gauntlet;
	
	EHandsMode CurrentMode = EHandsMode::Empty;
	float CurrentInteractionTime = 0.f;
	bool bCharging = false;
	float CurrentChargingTime = 0.0f;
	float ChargingTime = 1.f; 
public:
	//BASE
	UHandsControllerComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwitchInteractionPrompt(bool Switcher);

	//Interact
	UFUNCTION()
	void StartInteract();
	UFUNCTION()
	void ExecuteInteract();
	UFUNCTION()
	void ExecuteLongInteract() const;

	UFUNCTION()
	void StartLongSyncInteraction() const;
	UFUNCTION()
	void ExecuteTickLongSyncInteract(float Progress) const;
	UFUNCTION()
	void CompleteLongSyncInteract() const;
	UFUNCTION()
	void CancelLongSyncInteract() const;
	
	UFUNCTION()
	void CancelInteract();
	
	UFUNCTION()
	void StartAttack();
	UFUNCTION()
	void StartCharge();
	UFUNCTION()
	void Charge(float DeltaTime);
	UFUNCTION()
	void EndCharge();

	UFUNCTION()
	void SetHoldActor(AActor* Object) {HoldActor = Object;}
	
	UFUNCTION(BlueprintCallable)
	void SwitchCurrenTool(int ToolNumber);
	void Equip(const TScriptInterface<IWeapon>& Weapon, EArrowType ArrowType = EArrowType::Blunt);


	//Fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<IWeapon> ActiveWeapon;
	
	//Settings
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 500.f;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float MaxInteractionTime = 5.f;
	UPROPERTY(EditAnywhere, Category = "Throw")
	int Force = 1000;
};