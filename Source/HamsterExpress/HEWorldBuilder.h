#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HEWorldBuilder.generated.h"

class UStaticMesh;
class UMaterialInterface;

UCLASS()
class HAMSTEREXPRESS_API AHEWorldBuilder : public AActor
{
    GENERATED_BODY()
public:
    AHEWorldBuilder();
    virtual void BeginPlay() override;

private:
    void BuildLighting();
    void BuildTerrain();
    void BuildVillage();
    void BuildRoute();
    void BuildForest();
    AActor* SpawnMesh(UStaticMesh* Mesh, const FVector& Location, const FVector& Scale,
        const FRotator& Rotation = FRotator::ZeroRotator, const FLinearColor& Tint = FLinearColor::White,
        bool bCollision = true);
    void SetTint(AActor* Actor, const FLinearColor& Tint);

    UPROPERTY() UStaticMesh* Cube;
    UPROPERTY() UStaticMesh* Sphere;
    UPROPERTY() UStaticMesh* Cylinder;
    UPROPERTY() UStaticMesh* Cone;
    UPROPERTY() UMaterialInterface* BasicMaterial;
};
