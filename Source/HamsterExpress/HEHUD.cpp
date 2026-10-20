#include "HEHUD.h"
#include "HEGameModeBase.h"
#include "HEHamsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

void AHEHUD::ShowMessage(const FString& InMessage, float Duration)
{
    Message = InMessage;
    MessageUntil = GetWorld() ? GetWorld()->GetTimeSeconds() + Duration : Duration;
}

void AHEHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) return;

    AHEGameModeBase* GM = Cast<AHEGameModeBase>(UGameplayStatics::GetGameMode(this));
    AHEHamsterCharacter* Hamster = Cast<AHEHamsterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;
    UFont* Large = GEngine ? GEngine->GetMediumFont() : Font;

    const float Pad = 20.f;
    DrawRect(FLinearColor(0.02f,0.025f,0.03f,0.74f), Pad-10.f, Pad-10.f, 340.f, 150.f);
    DrawText(TEXT("HAMSTER EXPRESS"), FLinearColor(1.f,0.78f,0.26f), Pad, Pad, Large, 1.15f, false);
    const int32 Coins = GM ? GM->GetCoins() : 0;
    DrawText(FString::Printf(TEXT("Coins: %d"), Coins), FLinearColor::White, Pad, Pad+38.f, Font, 1.0f, false);

    FString Objective = TEXT("Objective: Find the parcel near the station and press E");
    if (GM && GM->IsDeliveryCompleted()) Objective = TEXT("Objective: Delivery complete - explore the village");
    else if (Hamster && Hamster->HasCargo()) Objective = TEXT("Objective: Reach the glowing destination beacon");
    DrawText(Objective, FLinearColor(0.85f,0.92f,1.f), Pad, Pad+65.f, Font, 0.9f, false);
    DrawText(TEXT("WASD Move  |  Shift Sprint  |  Mouse Camera  |  F5 Save  |  F9 Load  |  Alt+Enter Fullscreen"),
        FLinearColor(0.72f,0.76f,0.82f), Pad, Pad+105.f, Font, 0.73f, false);

    if (!Message.IsEmpty() && GetWorld()->GetTimeSeconds() < MessageUntil)
    {
        const float W = 620.f, H = 54.f;
        const float X = (Canvas->SizeX - W) * 0.5f;
        const float Y = Canvas->SizeY * 0.78f;
        DrawRect(FLinearColor(0.02f,0.025f,0.03f,0.82f), X, Y, W, H);
        DrawText(Message, FLinearColor::White, X+22.f, Y+17.f, Font, 1.0f, false);
    }
}
