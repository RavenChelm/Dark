// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Dark/InteractObjects/Interfaces/IDamageable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

#define SURFACE_Wood EPhysicalSurface::SurfaceType1
#define SURFACE_Metal EPhysicalSurface::SurfaceType2

AArrowProjectile::AArrowProjectile()
{
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    // CollisionComp->SetCollisionProfileName(TEXT("ArrowProjectile"));
    CollisionComp->SetGenerateOverlapEvents(true);
    CollisionComp->OnComponentHit.AddDynamic(this, &AArrowProjectile::OnHit);
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AArrowProjectile::OnOverlapBegin);
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
    
    const float FinalSpeed = ProjectileMovement->MaxSpeed;
    
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
    EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(SurfaceType)));
        
    if (Hit.PhysMaterial.IsValid())
    {
        if(SurfaceType == SURFACE_Metal)
        {
            Destroy();
            return;
        }
        if(SurfaceType == SURFACE_Wood)
        {
            AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
            ProjectileMovement->StopMovementImmediately();
            CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PM is not valid"));
    }

    if (GetWorld())
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), GetActorRotation());
    }
    
    if (OtherActor && OtherActor != this && OtherComp != nullptr)
    {
        if (TArray<UActorComponent*> Components = OtherActor->GetComponentsByInterface(UDamageable::StaticClass()); Components.Num() > 0)
        {
            for (UActorComponent* Comp : Components)
            {
                IDamageable::Execute_ApplyDamage(Comp, nullptr, this, ArrowType); // TODO:: Не будет ли вызываться в этом моменте не изменённая версия ArrowType?
            }
        }
    }
}

void AArrowProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bWasLaunched) return;
    
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
    FString::Printf(TEXT("Arrow overlapped with: %s"), *OtherActor->GetName()));
}
