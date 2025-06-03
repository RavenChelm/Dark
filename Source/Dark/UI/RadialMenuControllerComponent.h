#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RadialMenuControllerComponent.generated.h"

struct FInputActionValue;
class ADarkCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API URadialMenuControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URadialMenuControllerComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Radial Menu")
	TSubclassOf<class UUserWidget> RadialMenuWidgetClass;
	
	UFUNCTION()
	void HandleRadialMenuActionPressed();
	UFUNCTION()
	void HandleRadialMenuActionReleased();
	UFUNCTION()
	void HandleMouseInput();

	UPROPERTY()
	class URadialMenuBase* RadialMenuWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Radial Menu")
	float RadialMenuRadius = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Radial Menu")
	int NumSectors = 8;
	bool bIsRadialMenuActive = false;
	FVector2D MousePosition; 
	FVector2D MenuCenter;
	
	UPROPERTY()
	TWeakObjectPtr<ADarkCharacter> Character;
};