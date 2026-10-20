#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HEHamsterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class HAMSTEREXPRESS_API AHEHamsterCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AHEHamsterCharacter();
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;

    bool HasCargo() const { return bHasCargo; }
    void SetHasCargo(bool bNewHasCargo);

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void StartSprint();
    void StopSprint();
    void Interact();
    void SaveGame();
    void LoadGame();

    UPROPERTY() USpringArmComponent* CameraBoom;
    UPROPERTY() UCameraComponent* FollowCamera;

    UPROPERTY() UStaticMeshComponent* Body;
    UPROPERTY() UStaticMeshComponent* Belly;
    UPROPERTY() UStaticMeshComponent* Head;
    UPROPERTY() UStaticMeshComponent* EarL;
    UPROPERTY() UStaticMeshComponent* EarR;
    UPROPERTY() UStaticMeshComponent* EyeL;
    UPROPERTY() UStaticMeshComponent* EyeR;
    UPROPERTY() UStaticMeshComponent* Nose;
    UPROPERTY() UStaticMeshComponent* PawL;
    UPROPERTY() UStaticMeshComponent* PawR;
    UPROPERTY() UStaticMeshComponent* FootL;
    UPROPERTY() UStaticMeshComponent* FootR;
    UPROPERTY() UStaticMeshComponent* Backpack;

    UPROPERTY() USceneComponent* CartRoot;
    UPROPERTY() UStaticMeshComponent* CartBed;
    UPROPERTY() UStaticMeshComponent* WheelL;
    UPROPERTY() UStaticMeshComponent* WheelR;
    UPROPERTY() UStaticMeshComponent* CartCargo;

    bool bHasCargo = false;
};
