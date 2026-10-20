#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HEGameSave.generated.h"

UCLASS()
class HAMSTEREXPRESS_API UHEGameSave : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY() FVector PlayerLocation = FVector(0.f, 0.f, 150.f);
    UPROPERTY() FRotator PlayerRotation = FRotator::ZeroRotator;
    UPROPERTY() int32 Coins = 0;
    UPROPERTY() bool bDeliveryCompleted = false;
};
