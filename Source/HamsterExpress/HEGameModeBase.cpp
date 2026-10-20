#include "HEGameModeBase.h"
#include "HEHamsterCharacter.h"
#include "HEHUD.h"
#include "HEWorldBuilder.h"
#include "HECargo.h"
#include "HEDeliveryGoal.h"
#include "HEGameSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AHEGameModeBase::AHEGameModeBase()
{
    DefaultPawnClass = AHEHamsterCharacter::StaticClass();
    HUDClass = AHEHUD::StaticClass();
}

void AHEGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    if (UWorld* W = GetWorld())
    {
        W->SpawnActor<AHEWorldBuilder>(FVector::ZeroVector, FRotator::ZeroRotator);
        W->SpawnActor<AHECargo>(FVector(260.f, 40.f, 70.f), FRotator(0,18,0));
        W->SpawnActor<AHEDeliveryGoal>(FVector(2500.f, 1000.f, 22.f), FRotator::ZeroRotator);
    }
    if (AHEHamsterCharacter* Hamster = Cast<AHEHamsterCharacter>(UGameplayStatics::GetPlayerPawn(this,0)))
    {
        Hamster->SetActorLocation(FVector(0.f,0.f,150.f));
    }
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void AHEGameModeBase::CompleteDelivery(AHEHamsterCharacter* Hamster)
{
    if (!Hamster || bDeliveryCompleted) return;
    bDeliveryCompleted = true;
    Coins += 100;
    Hamster->SetHasCargo(false);
    if (APlayerController* PC = Cast<APlayerController>(Hamster->GetController()))
    {
        if (AHEHUD* HUD = Cast<AHEHUD>(PC->GetHUD()))
            HUD->ShowMessage(TEXT("Delivery complete! +100 coins. The route is connected again."), 5.f);
    }
    SaveProgress(Hamster);
}

void AHEGameModeBase::SaveProgress(AHEHamsterCharacter* Hamster)
{
    if (!Hamster) return;
    UHEGameSave* Save = Cast<UHEGameSave>(UGameplayStatics::CreateSaveGameObject(UHEGameSave::StaticClass()));
    if (!Save) return;
    Save->PlayerLocation = Hamster->GetActorLocation();
    Save->PlayerRotation = Hamster->GetActorRotation();
    Save->Coins = Coins;
    Save->bDeliveryCompleted = bDeliveryCompleted;
    UGameplayStatics::SaveGameToSlot(Save, SaveSlot, 0);
    if (APlayerController* PC = Cast<APlayerController>(Hamster->GetController()))
        if (AHEHUD* HUD = Cast<AHEHUD>(PC->GetHUD())) HUD->ShowMessage(TEXT("Game saved."), 2.f);
}

void AHEGameModeBase::LoadProgress(AHEHamsterCharacter* Hamster)
{
    if (!Hamster || !UGameplayStatics::DoesSaveGameExist(SaveSlot,0)) return;
    UHEGameSave* Save = Cast<UHEGameSave>(UGameplayStatics::LoadGameFromSlot(SaveSlot,0));
    if (!Save) return;
    Coins = Save->Coins;
    bDeliveryCompleted = Save->bDeliveryCompleted;
    Hamster->SetActorLocationAndRotation(Save->PlayerLocation, Save->PlayerRotation);
    Hamster->SetHasCargo(false);
    if (APlayerController* PC = Cast<APlayerController>(Hamster->GetController()))
        if (AHEHUD* HUD = Cast<AHEHUD>(PC->GetHUD())) HUD->ShowMessage(TEXT("Save loaded."), 2.f);
}
