// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Hands/CrossbowSkeletalMeshComponent.h"
#include "Hands/ElectricGauntletSkeletalMeshComponent.h"
#include "Hands/HandsControllerComponent.h"
#include "MovementSystem/CustomCharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "UI/RadialMenuControllerComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADarkCharacter

ADarkCharacter::ADarkCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Create Hands Component
	/*PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));*/
	HandsController = CreateDefaultSubobject<UHandsControllerComponent>(TEXT("HandsController"));
	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
	GrabPoint->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	CrossbowComponent = CreateDefaultSubobject<UCrossbowSkeletalMeshComponent>(TEXT("CrossbowComponent"));
	CrossbowComponent->SetOnlyOwnerSee(true);
	CrossbowComponent->SetupAttachment(FirstPersonCameraComponent);
	CrossbowComponent->bCastDynamicShadow = false;
	CrossbowComponent->CastShadow = false;
	CrossbowComponent->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GauntletComponent = CreateDefaultSubobject<UElectricGauntletSkeletalMeshComponent>(TEXT("SwordComponent"));
	GauntletComponent->SetOnlyOwnerSee(true);
	GauntletComponent->SetupAttachment(FirstPersonCameraComponent);
	GauntletComponent->bCastDynamicShadow = false;
	GauntletComponent->CastShadow = false;
	GauntletComponent->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	// Get a MovementComponent
	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());

	//Create a RadialMenuController
	RadialMenuController = CreateDefaultSubobject<URadialMenuControllerComponent>(TEXT("RadialMenuController"));
}

//////////////////////////////////////////////////////////////////////////// Input

void ADarkCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADarkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADarkCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADarkCharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADarkCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADarkCharacter::Look);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ADarkCharacter::StartSprint); 
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADarkCharacter::StopSprint);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADarkCharacter::StartCrouch);
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADarkCharacter::StartAttack);
		
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Started, this, &ADarkCharacter::StartCharge);
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Completed, this, &ADarkCharacter::ReleaseCharge);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADarkCharacter::StartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ADarkCharacter::ReleaseInteract);
		
		EnhancedInputComponent->BindAction(RadialMenuAction, ETriggerEvent::Started, this, &ADarkCharacter::RadialMenuPressed);
		EnhancedInputComponent->BindAction(RadialMenuAction, ETriggerEvent::Completed, this, &ADarkCharacter::RadialMenuReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADarkCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADarkCharacter::Look(const FInputActionValue& Value)
{
	OnMouseInput.Broadcast();

	if (Controller != nullptr && !RadialMenuController->bIsRadialMenuActive)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADarkCharacter::StartInteract()
{
	OnInteract.Broadcast();
}

void ADarkCharacter::ReleaseInteract()
{
	OnReleaseInteract.Broadcast();
}

void ADarkCharacter::StartSprint()
{
	OnSprintStart.Broadcast();
}

void ADarkCharacter::StopSprint()
{
	OnSprintStop.Broadcast();
}

void ADarkCharacter::StartCrouch()
{
	OnCrouch.Broadcast();
}

void ADarkCharacter::StartAttack()
{
	OnAttack.Broadcast();
}
void ADarkCharacter::StartCharge()
{
	OnStartCharge.Broadcast();
}
void ADarkCharacter::ReleaseCharge()
{
	OnReleaseCharge.Broadcast();
}
void ADarkCharacter::Jump()
{
	OnJumpStart.Broadcast();
	Super::Jump();
}

void ADarkCharacter::StopJumping()
{
	Super::StopJumping();
}

void ADarkCharacter::RadialMenuPressed()
{
	OnRadialMenuActionPressed.Broadcast();
}

void ADarkCharacter::RadialMenuReleased()
{
	OnRadialMenuActionReleased.Broadcast();
}

