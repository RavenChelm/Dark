// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "IThrowable.h"
#include "Components/ActorComponent.h"
#include "MovableObjectComponent.generated.h"


class ADarkCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, Interfaces="Interactable") )
class DARK_API UMovableObjectComponent : public UActorComponent, public IInteractable, public IThrowable
{
	GENERATED_BODY()
	UPROPERTY()
	const ADarkCharacter* Character;
public:	
	UMovableObjectComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Interact_Implementation(const AActor* Other) override;
	virtual void Throw_Implementation(const FVector& Direction, float Force) override;
	virtual void Drop_Implementation() override;
};
