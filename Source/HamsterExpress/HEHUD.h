#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HEHUD.generated.h"

UCLASS()
class HAMSTEREXPRESS_API AHEHUD : public AHUD
{
    GENERATED_BODY()
public:
    virtual void DrawHUD() override;
    void ShowMessage(const FString& InMessage, float Duration = 3.f);
private:
    FString Message;
    float MessageUntil = 0.f;
};
