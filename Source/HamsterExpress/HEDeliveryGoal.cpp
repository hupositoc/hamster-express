#include "HEDeliveryGoal.h"
#include "HEHamsterCharacter.h"
#include "HEGameModeBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

AHEDeliveryGoal::AHEDeliveryGoal()
{
    PrimaryActorTick.bCanEverTick = false;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(Root);
    Trigger->InitSphereRadius(180.f);
    Trigger->SetCollisionProfileName(TEXT("Trigger"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
    Base->SetupAttachment(Root);
    if (Cylinder.Succeeded()) Base->SetStaticMesh(Cylinder.Object);
    Base->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.12f));
    Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Beacon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beacon"));
    Beacon->SetupAttachment(Root);
    if (Sphere.Succeeded()) Beacon->SetStaticMesh(Sphere.Object);
    Beacon->SetRelativeLocation(FVector(0.f,0.f,120.f));
    Beacon->SetRelativeScale3D(FVector(0.35f));
    Beacon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHEDeliveryGoal::BeginPlay()
{
    Super::BeginPlay();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AHEDeliveryGoal::OnGoalOverlap);
    UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (Mat)
    {
        if (UMaterialInstanceDynamic* M = UMaterialInstanceDynamic::Create(Mat, Base)) { M->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.12f,0.22f,0.28f)); Base->SetMaterial(0,M); }
        if (UMaterialInstanceDynamic* M = UMaterialInstanceDynamic::Create(Mat, Beacon)) { M->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.20f,0.82f,1.0f)); Beacon->SetMaterial(0,M); }
    }
}

void AHEDeliveryGoal::OnGoalOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    AHEHamsterCharacter* Hamster = Cast<AHEHamsterCharacter>(OtherActor);
    if (!Hamster || !Hamster->HasCargo()) return;
    if (AHEGameModeBase* GM = Cast<AHEGameModeBase>(UGameplayStatics::GetGameMode(this)))
    {
        GM->CompleteDelivery(Hamster);
    }
}
