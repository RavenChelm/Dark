// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Dark/InteractObjects/Interfaces/IInteractable.h"
#include "Interfaces/IReactive.h"
#include "IgnitableObjectComponent.generated.h"

class UParticleSystemComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, Interfaces="Interactable"))
class DARK_API UIgnitableObjectComponent : public UActorComponent, public IReactive
{
	GENERATED_BODY()
	bool bIsBurn = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess=true))
	UParticleSystemComponent* FlameParticleComponent;
public:
	UIgnitableObjectComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual bool ReactToElement_Implementation(EElementalType& ElementType, AActor* Instigator, const FHitResult& Hit) override;
	void Ignition();
	void Extinguish();
	void SpawnSteamCloud(AActor* Instigator, const FHitResult& Hit);


	UFUNCTION(BlueprintCallable)
	void SetFlameParticleSystemComponent(UParticleSystemComponent* PSC) {FlameParticleComponent = PSC;};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> SteamCloud;
};
