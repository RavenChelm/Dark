// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkArrowProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/GameEngine.h"

ADarkArrowProjectile::ADarkArrowProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("ArrowProjectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADarkArrowProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;


	InitialLifeSpan = 0.0f;

}

void ADarkArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit"));
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Do Something"));
	}
}


void ADarkArrowProjectile::Initialize(float PartVelocity) {
	FVector speed = ProjectileMovement->Velocity;
	ProjectileMovement->Velocity = speed * PartVelocity ;
	ProjectileMovement->UpdateComponentVelocity();
}
