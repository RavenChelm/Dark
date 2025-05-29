// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "Components/ActorComponent.h"
#include "PickUpItemComponent.generated.h"

UENUM(BlueprintType)
enum class EPickUpType : uint8
{
	Arrow_Blunt               UMETA(DisplayName = "Blunt Arrow"),
	Arrow_Fire				  UMETA(DisplayName = "Fire Arrow"),
	Arrow_Water               UMETA(DisplayName = "Water Arrow"),
	Food					  UMETA(DisplayName = "Food"),
	Coins					  UMETA(DisplayName = "Coins"),
	Keys					  UMETA(DisplayName = "Keys"),
	Note					  UMETA(DisplayName = "Note"),
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARK_API UPickUpItemComponent : public UActorComponent, public IInteractable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	UPickUpItemComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Interact_Implementation(const AActor* Other) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickUpType PickUpType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (EditCondition = "PickupType == EPickupType::Arrow_Blunt || PickupType == EPickupType::Arrow_Fire || PickupType == EPickupType::Arrow_Water"))
	int32 AmmoCount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (EditCondition = "PickupType == EPickupType::Food"))
	float FoodRestoreAmount = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (EditCondition = "PickupType == EPickupType::Coins"))
	int CoinsAmount = 1;
};
