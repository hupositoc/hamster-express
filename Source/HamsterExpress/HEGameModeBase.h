#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HEGameModeBase.generated.h"

class AHEHamsterCharacter;

UCLASS()
class HAMSTEREXPRESS_API AHEGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    AHEGameModeBase();
    virtual void BeginPlay() override;

    int32 GetCoins() const { return Coins; }
    bool IsDeliveryCompleted() const { return bDeliveryCompleted; }
    void CompleteDelivery(AHEHamsterCharacter* Hamster);
    void SaveProgress(AHEHamsterCharacter* Hamster);
    void LoadProgress(AHEHamsterCharacter* Hamster);

private:
    int32 Coins = 0;
    bool bDeliveryCompleted = false;
    FString SaveSlot = TEXT("HamsterExpress_Auto");
};
