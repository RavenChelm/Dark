// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IReactive.h"
#include "ElementalWaterInteractableObjectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UElementalWaterInteractableObjectComponent : public UActorComponent, public IReactive
{
	GENERATED_BODY()

public:
	UElementalWaterInteractableObjectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReactToElement_Implementation(EElementalType ElementType, AActor* Instigator, const FHitResult& Hit) override;
	void SpawnSteamCloud(AActor* Instigator, const FHitResult& Hit);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> SteamCloud;
};
