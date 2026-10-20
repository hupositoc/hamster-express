#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HEDeliveryGoal.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class HAMSTEREXPRESS_API AHEDeliveryGoal : public AActor
{
    GENERATED_BODY()
public:
    AHEDeliveryGoal();
protected:
    virtual void BeginPlay() override;
private:
    UFUNCTION() void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY() USceneComponent* Root;
    UPROPERTY() USphereComponent* Trigger;
    UPROPERTY() UStaticMeshComponent* Base;
    UPROPERTY() UStaticMeshComponent* Beacon;
};
