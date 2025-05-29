// Fill out your copyright notice in the Description page of Project Settings.


#include "FireArrowProjectile.h"
#include "../InteractObjects/WaterInteractObject.h"

void AFireArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit Fire Arrow"));
		UWaterInteractObject* InteractObject = OtherActor->GetComponentByClass<UWaterInteractObject>();
		if (InteractObject != nullptr) {
			InteractObject->ActStateUp();
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Fire Source is Light"));
		}
		Destroy();
	}
}
