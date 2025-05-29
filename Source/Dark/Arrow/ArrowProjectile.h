// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class DARK_API AArrowProjectile : public AActor
{
	GENERATED_BODY()
    
public:
	AArrowProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Launch(const FVector& Direction, float SpeedMultiplier = 1.0f);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	bool bWasLaunched = false;
	UPROPERTY(EditDefaultsOnly, Category = "ArrowSettings")
	bool ItSharpArrow = false;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactEffect;
};

