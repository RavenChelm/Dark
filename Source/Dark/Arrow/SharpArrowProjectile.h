// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrowProjectile.h"
#include "SharpArrowProjectile.generated.h"

UCLASS()
class DARK_API ASharpArrowProjectile : public AArrowProjectile
{
	GENERATED_BODY()

public:
	ASharpArrowProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
