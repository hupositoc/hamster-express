#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HECargo.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class HAMSTEREXPRESS_API AHECargo : public AActor
{
    GENERATED_BODY()
public:
    AHECargo();
    virtual void BeginPlay() override;
    void Pickup();
    bool IsPickedUp() const { return bPickedUp; }
private:
    UPROPERTY() USceneComponent* Root;
    UPROPERTY() UStaticMeshComponent* Box;
    UPROPERTY() UStaticMeshComponent* RibbonA;
    UPROPERTY() UStaticMeshComponent* RibbonB;
    bool bPickedUp = false;
};
