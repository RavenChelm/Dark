// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkBowComponent.h"
#include "DarkCharacter.h"
#include "Arrow/DarkArrowProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"

UDarkBowComponent::UDarkBowComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

bool UDarkBowComponent::AttachWeapon(ADarkCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UDarkBowComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UDarkBowComponent::PullArrow);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UDarkBowComponent::ShootArrow);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &UDarkBowComponent::ShootArrow);
		}
	}

	return true;
}

void UDarkBowComponent::PullArrow()
{
	if (Character == nullptr || Character->GetController() == nullptr) return;

	if (ArrowForceCurrent < ArrowForceMax) {
		ArrowForceCurrent += ArrowForceIncrement * GetWorld()->DeltaTimeSeconds;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Pulling"));
	}

	if (ArrowForceCurrent > ArrowForceMax) {
		ArrowForceCurrent = ArrowForceMax;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Max Force"));
	}

	//start shaking/pull animation and sound
}
	
void UDarkBowComponent::ShootArrow()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			// Spawn the projectile at the muzzle
			ADarkArrowProjectile* Arrow = World->SpawnActor<ADarkArrowProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Arrow->Initialize(ArrowForceCurrent);
			ArrowForceCurrent = 0;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Fire"));
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UDarkBowComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (Character != nullptr)
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}
