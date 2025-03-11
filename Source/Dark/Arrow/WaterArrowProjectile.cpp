// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterArrowProjectile.h"
#include "../InteractObjects/WaterInteractObject.h"

void AWaterArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit Water Arrow"));
		UWaterInteractObject* InteractObject = OtherActor->GetComponentByClass<UWaterInteractObject>();
		if (InteractObject != nullptr) {
			InteractObject->ActStateDown();
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Fire Source is extinguished"));
		}
		Destroy();
	}
}