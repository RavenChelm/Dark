// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrowProjectile.h"
#include "WaterArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API AWaterArrowProjectile : public AArrowProjectile
{
	GENERATED_BODY()
public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	bool CanSpawnWaterPuddle(const FHitResult& Hit) const;
	void SpawnWaterPuddle(const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> WaterPuddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPuddleAngle = 45.0f;
};
