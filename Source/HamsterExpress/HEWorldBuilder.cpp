#include "HEWorldBuilder.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/SkyAtmosphere.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AHEWorldBuilder::AHEWorldBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderObj(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeObj(TEXT("/Engine/BasicShapes/Cone.Cone"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatObj(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    Cube = CubeObj.Succeeded() ? CubeObj.Object : nullptr;
    Sphere = SphereObj.Succeeded() ? SphereObj.Object : nullptr;
    Cylinder = CylinderObj.Succeeded() ? CylinderObj.Object : nullptr;
    Cone = ConeObj.Succeeded() ? ConeObj.Object : nullptr;
    BasicMaterial = MatObj.Succeeded() ? MatObj.Object : nullptr;
}

void AHEWorldBuilder::BeginPlay()
{
    Super::BeginPlay();
    BuildLighting();
    BuildTerrain();
    BuildVillage();
    BuildRoute();
    BuildForest();
}

AActor* AHEWorldBuilder::SpawnMesh(UStaticMesh* Mesh, const FVector& Location, const FVector& Scale,
    const FRotator& Rotation, const FLinearColor& Tint, bool bCollision)
{
    if (!Mesh || !GetWorld()) return nullptr;
    AStaticMeshActor* Actor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation);
    if (!Actor) return nullptr;
    UStaticMeshComponent* C = Actor->GetStaticMeshComponent();
    C->SetMobility(EComponentMobility::Movable);
    C->SetStaticMesh(Mesh);
    C->SetWorldScale3D(Scale);
    C->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    C->SetCastShadow(true);
    if (BasicMaterial)
    {
        UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BasicMaterial, Actor);
        if (MID)
        {
            MID->SetVectorParameterValue(TEXT("Color"), Tint);
            C->SetMaterial(0, MID);
        }
    }
    return Actor;
}

void AHEWorldBuilder::BuildLighting()
{
    UWorld* W = GetWorld(); if (!W) return;
    ADirectionalLight* Sun = W->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-42.f, -28.f, 0.f));
    if (Sun)
    {
        Sun->GetLightComponent()->SetIntensity(7.5f);
        Sun->GetLightComponent()->SetLightColor(FLinearColor(1.f,0.92f,0.78f));
        Sun->GetLightComponent()->SetCastShadows(true);
    }
    ASkyLight* Sky = W->SpawnActor<ASkyLight>();
    if (Sky)
    {
        Sky->GetLightComponent()->SetIntensity(0.75f);
        Sky->GetLightComponent()->SetMobility(EComponentMobility::Movable);
        Sky->GetLightComponent()->SetRealTimeCapture(true);
    }
    W->SpawnActor<ASkyAtmosphere>();
    AExponentialHeightFog* Fog = W->SpawnActor<AExponentialHeightFog>();
    if (Fog) Fog->GetComponent()->SetFogDensity(0.0045f);

    APostProcessVolume* PP = W->SpawnActor<APostProcessVolume>();
    if (PP)
    {
        PP->bUnbound = true;
        PP->Settings.bOverride_BloomIntensity = true;
        PP->Settings.BloomIntensity = 0.35f;
        PP->Settings.bOverride_VignetteIntensity = true;
        PP->Settings.VignetteIntensity = 0.16f;
    }
}

void AHEWorldBuilder::BuildTerrain()
{
    SpawnMesh(Cube, FVector(1250,400,-65), FVector(35.f,28.f,0.65f), FRotator::ZeroRotator, FLinearColor(0.23f,0.48f,0.24f), true);
    SpawnMesh(Cube, FVector(1250,400,-12), FVector(30.f,1.0f,0.06f), FRotator(0,22,0), FLinearColor(0.52f,0.39f,0.24f), true);

    // river + banks
    SpawnMesh(Cube, FVector(1450,-820,-26), FVector(18.f,3.2f,0.10f), FRotator(0,8,0), FLinearColor(0.08f,0.38f,0.58f), false);
    SpawnMesh(Cube, FVector(1450,-1160,-30), FVector(18.f,0.6f,0.22f), FRotator(0,8,0), FLinearColor(0.30f,0.28f,0.19f), true);
    SpawnMesh(Cube, FVector(1450,-480,-30), FVector(18.f,0.6f,0.22f), FRotator(0,8,0), FLinearColor(0.30f,0.28f,0.19f), true);

    // distant rounded hills
    for (int32 i=0;i<11;i++)
    {
        float X = -900.f + i*450.f;
        float Y = 2350.f + FMath::Sin(i*1.7f)*350.f;
        SpawnMesh(Sphere, FVector(X,Y,-110.f), FVector(5.5f,4.5f,2.0f), FRotator::ZeroRotator,
            FLinearColor(0.18f+0.01f*i,0.34f+0.008f*i,0.22f), true);
    }
}

void AHEWorldBuilder::BuildVillage()
{
    // Delivery station: warm landmark near spawn.
    SpawnMesh(Cube, FVector(-250,180,130), FVector(2.2f,1.7f,1.5f), FRotator::ZeroRotator, FLinearColor(0.62f,0.30f,0.16f), true);
    SpawnMesh(Cone, FVector(-250,180,330), FVector(2.8f,2.4f,1.2f), FRotator::ZeroRotator, FLinearColor(0.20f,0.12f,0.09f), true);
    SpawnMesh(Cube, FVector(-55,180,90), FVector(0.13f,0.65f,0.90f), FRotator::ZeroRotator, FLinearColor(0.10f,0.07f,0.05f), false);
    SpawnMesh(Cube, FVector(-110,180,235), FVector(0.12f,0.75f,0.22f), FRotator::ZeroRotator, FLinearColor(0.92f,0.72f,0.30f), false);

    const FVector HouseLocs[] = {
        FVector(250,520,100), FVector(600,650,100), FVector(920,460,100),
        FVector(520,-380,100), FVector(920,-500,100), FVector(-500,-420,100)
    };
    for (int32 i=0;i<6;i++)
    {
        const FVector P=HouseLocs[i];
        const FLinearColor Wall = (i%2==0) ? FLinearColor(0.70f,0.50f,0.31f) : FLinearColor(0.55f,0.36f,0.22f);
        SpawnMesh(Cube,P,FVector(1.55f,1.25f,1.10f),FRotator::ZeroRotator,Wall,true);
        SpawnMesh(Cone,P+FVector(0,0,155),FVector(2.0f,1.7f,0.85f),FRotator::ZeroRotator,FLinearColor(0.26f,0.13f,0.08f),true);
        SpawnMesh(Cube,P+FVector(145,0,-8),FVector(0.12f,0.40f,0.70f),FRotator::ZeroRotator,FLinearColor(0.12f,0.08f,0.05f),false);
    }

    // village lamps / signposts
    for (int32 i=0;i<8;i++)
    {
        FVector P(100.f+i*220.f, 150.f + ((i%2)?-180.f:180.f), 55.f);
        SpawnMesh(Cylinder,P,FVector(0.07f,0.07f,0.9f),FRotator::ZeroRotator,FLinearColor(0.12f,0.10f,0.08f),false);
        SpawnMesh(Sphere,P+FVector(0,0,95),FVector(0.16f),FRotator::ZeroRotator,FLinearColor(1.0f,0.68f,0.22f),false);
    }
}

void AHEWorldBuilder::BuildRoute()
{
    // stepping stones and readable route rhythm
    for (int32 i=0;i<13;i++)
    {
        const float T = i/12.f;
        FVector P = FMath::Lerp(FVector(250,0,4), FVector(2500,1000,4), T);
        P.Y += FMath::Sin(T*PI*2.f)*90.f;
        SpawnMesh(Cube,P,FVector(0.62f,0.38f,0.05f),FRotator(0,24,0),FLinearColor(0.50f,0.36f,0.22f),true);
    }
    // bridge crossing toward river side path
    for (int32 i=0;i<7;i++)
    {
        SpawnMesh(Cube,FVector(1220+i*85,-780,22),FVector(0.72f,0.55f,0.08f),FRotator::ZeroRotator,FLinearColor(0.45f,0.25f,0.12f),true);
    }
}

void AHEWorldBuilder::BuildForest()
{
    FRandomStream Rng(1337);
    for (int32 i=0;i<55;i++)
    {
        FVector P(Rng.FRandRange(-1100.f,3600.f),Rng.FRandRange(-2200.f,2200.f),0.f);
        if (P.Size2D() < 650.f || FVector2D(P.X-2500,P.Y-1000).Size()<500.f) continue;
        const float S=Rng.FRandRange(0.75f,1.35f);
        SpawnMesh(Cylinder,P+FVector(0,0,85*S),FVector(0.16f*S,0.16f*S,0.85f*S),FRotator::ZeroRotator,FLinearColor(0.23f,0.12f,0.06f),true);
        SpawnMesh(Cone,P+FVector(0,0,215*S),FVector(0.85f*S,0.85f*S,1.25f*S),FRotator::ZeroRotator,
            FLinearColor(Rng.FRandRange(0.10f,0.18f),Rng.FRandRange(0.30f,0.48f),Rng.FRandRange(0.12f,0.22f)),true);
    }
    for (int32 i=0;i<28;i++)
    {
        FVector P(Rng.FRandRange(-800.f,3400.f),Rng.FRandRange(-1900.f,1900.f),Rng.FRandRange(5.f,20.f));
        const float S=Rng.FRandRange(0.25f,0.7f);
        SpawnMesh(Sphere,P,FVector(S,S*0.8f,S*0.55f),FRotator(Rng.FRandRange(-10,10),Rng.FRandRange(0,180),0),FLinearColor(0.26f,0.27f,0.25f),true);
    }
}
