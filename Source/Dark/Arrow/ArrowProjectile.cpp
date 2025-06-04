// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

AArrowProjectile::AArrowProjectile()
{
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    // CollisionComp->SetCollisionProfileName(TEXT("ArrowProjectile"));
    CollisionComp->OnComponentHit.AddDynamic(this, &AArrowProjectile::OnHit);
    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 0.f; // Start with 0, will be set in Launch
    ProjectileMovement->MaxSpeed = 10000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.1f; 
    
}

void AArrowProjectile::Launch(const FVector& Direction, float SpeedMultiplier)
{
    if(!ProjectileMovement || bWasLaunched) return;
    
    const float FinalSpeed = ProjectileMovement->MaxSpeed * SpeedMultiplier;
    
    ProjectileMovement->Velocity = Direction.GetSafeNormal() * FinalSpeed;
    ProjectileMovement->Activate();
    
    bWasLaunched = true;

    // SetLifeSpan(5.0f); // TODO:: Object Pull
}

void AArrowProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bWasLaunched) return;

    AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
    
    ProjectileMovement->StopMovementImmediately();
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (GetWorld())
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ImpactEffect,
            GetActorLocation(),
            GetActorRotation()
        );
    }
}
