#include "HECargo.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

AHECargo::AHECargo()
{
    PrimaryActorTick.bCanEverTick = false;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CargoBox"));
    Box->SetupAttachment(Root);
    if (Cube.Succeeded()) Box->SetStaticMesh(Cube.Object);
    Box->SetRelativeScale3D(FVector(0.55f, 0.42f, 0.42f));
    Box->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    RibbonA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RibbonA"));
    RibbonA->SetupAttachment(Box);
    if (Cube.Succeeded()) RibbonA->SetStaticMesh(Cube.Object);
    RibbonA->SetRelativeScale3D(FVector(0.08f, 1.03f, 1.03f));
    RibbonA->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RibbonB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RibbonB"));
    RibbonB->SetupAttachment(Box);
    if (Cube.Succeeded()) RibbonB->SetStaticMesh(Cube.Object);
    RibbonB->SetRelativeScale3D(FVector(1.03f, 0.08f, 1.03f));
    RibbonB->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHECargo::Pickup()
{
    if (bPickedUp) return;
    bPickedUp = true;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

void AHECargo::BeginPlay()
{
    Super::BeginPlay();
    UMaterialInterface* Base = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (!Base) return;
    auto Tint = [Base](UStaticMeshComponent* C, const FLinearColor& Col)
    {
        if (UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Base, C))
        {
            MID->SetVectorParameterValue(TEXT("Color"), Col);
            C->SetMaterial(0, MID);
        }
    };
    Tint(Box, FLinearColor(0.74f,0.51f,0.20f));
    Tint(RibbonA, FLinearColor(0.72f,0.08f,0.10f));
    Tint(RibbonB, FLinearColor(0.72f,0.08f,0.10f));
}
