#include "RadialMenuControllerComponent.h"

#include "InputActionValue.h"
#include "RadialMenuBase.h"
#include "Dark/DarkCharacter.h"

URadialMenuControllerComponent::URadialMenuControllerComponent()
{

}

void URadialMenuControllerComponent::BeginPlay()
{
    Super::BeginPlay();
   
    Character = Cast<ADarkCharacter>(GetOwner());
    if (Character.Get())
    {
        Character->OnRadialMenuActionPressed.AddDynamic(this, &URadialMenuControllerComponent::HandleRadialMenuActionPressed);
        Character->OnRadialMenuActionReleased.AddDynamic(this, &URadialMenuControllerComponent::HandleRadialMenuActionReleased);
        Character->OnMouseInput.AddDynamic(this, &URadialMenuControllerComponent::HandleMouseInput);
    }
    if (APlayerController* PC = Character->GetController<APlayerController>())
    {
        RadialMenuWidget = CreateWidget<URadialMenuBase>(PC, RadialMenuWidgetClass);
        if (RadialMenuWidget)
        {
            RadialMenuWidget->AddToViewport();
            RadialMenuWidget->SetVisibility(ESlateVisibility::Hidden);
            
        }
    }
   
}

void URadialMenuControllerComponent::HandleRadialMenuActionPressed()
{
    if (APlayerController* PC = Character->GetController<APlayerController>(); RadialMenuWidget && PC)
    {
        FVector2D ViewportSize;
        GEngine->GameViewport->GetViewportSize(ViewportSize);
        
        MenuCenter = ViewportSize / 2;
        MousePosition = MenuCenter;
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
            FString::Printf(TEXT("ViewportSize X: %f Y:%f"), ViewportSize.X, ViewportSize.Y));

        RadialMenuWidget->SetVisibility(ESlateVisibility::Visible);
        PC->SetMouseLocation(MenuCenter.X, MenuCenter.Y);
        MousePosition = MenuCenter;
        bIsRadialMenuActive = true;
        PC->bShowMouseCursor = true;
    }
}

void URadialMenuControllerComponent::HandleRadialMenuActionReleased()
{
    if (APlayerController* PC = Character->GetController<APlayerController>(); RadialMenuWidget && PC)
    {
        RadialMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        RadialMenuWidget->ActivateSelectedSection(GetOwner());
        PC->bShowMouseCursor = false;
        bIsRadialMenuActive = false;
    }
}

void URadialMenuControllerComponent::HandleMouseInput()
{
    if (!bIsRadialMenuActive) return;
    float MouseX, MouseY;
    if (const APlayerController* PC = Character->GetController<APlayerController>(); PC->GetMousePosition(MouseX, MouseY))
    {
        if (GEngine && GEngine->GameViewport)
        {
            FVector2D ViewportSize;
            GEngine->GameViewport->GetViewportSize(ViewportSize);
            MousePosition.X = FMath::Clamp(MouseX, 0.0f, ViewportSize.X);
            MousePosition.Y = FMath::Clamp(MouseY, 0.0f, ViewportSize.Y);
        }
    }
   

    const FVector2D Direction = MousePosition - MenuCenter;
    if (const float Distance = Direction.Size(); Distance < RadialMenuRadius * 0.2f) 
        return;
    
    const float AngleRad = FMath::Atan2(-Direction.Y, Direction.X);
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);
    if (AngleDeg < 0) AngleDeg += 360.f;


    const float SectorSize = 360.f / NumSectors;
    int32 SectorIndex = FMath::FloorToInt(AngleDeg / SectorSize) % NumSectors;
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
    FString::Printf(TEXT("SectorIndex : %d"), SectorIndex));

    RadialMenuWidget->UpdateSelection(SectorIndex);
}

void URadialMenuControllerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (RadialMenuWidget)
    {
        RadialMenuWidget->RemoveFromParent();
        RadialMenuWidget = nullptr;
    }
    
    Super::EndPlay(EndPlayReason);
}