#include "HEHamsterCharacter.h"
#include "HECargo.h"
#include "HEGameModeBase.h"
#include "HEHUD.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

static void ConfigureVisual(UStaticMeshComponent* Comp, UStaticMesh* Mesh, const FVector& Scale, const FVector& Loc)
{
    if (Mesh) Comp->SetStaticMesh(Mesh);
    Comp->SetRelativeScale3D(Scale);
    Comp->SetRelativeLocation(Loc);
    Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Comp->SetCastShadow(true);
}

AHEHamsterCharacter::AHEHamsterCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(34.f, 58.f);
    GetCharacterMovement()->MaxWalkSpeed = 420.f;
    GetCharacterMovement()->MaxAcceleration = 1500.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1250.f;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 620.f, 0.f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 420.f;
    CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 55.f));
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 12.f;
    CameraBoom->bDoCollisionTest = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->FieldOfView = 72.f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderObj(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    UStaticMesh* Sphere = SphereObj.Succeeded() ? SphereObj.Object : nullptr;
    UStaticMesh* Cube = CubeObj.Succeeded() ? CubeObj.Object : nullptr;
    UStaticMesh* Cylinder = CylinderObj.Succeeded() ? CylinderObj.Object : nullptr;

    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body")); Body->SetupAttachment(RootComponent);
    ConfigureVisual(Body, Sphere, FVector(0.46f,0.39f,0.53f), FVector(0,0,0));

    Belly = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Belly")); Belly->SetupAttachment(Body);
    ConfigureVisual(Belly, Sphere, FVector(0.74f,0.79f,0.72f), FVector(22.f,0.f,-5.f));

    Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head")); Head->SetupAttachment(RootComponent);
    ConfigureVisual(Head, Sphere, FVector(0.48f,0.44f,0.46f), FVector(18.f,0.f,58.f));

    EarL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarL")); EarL->SetupAttachment(Head);
    ConfigureVisual(EarL, Sphere, FVector(0.24f,0.16f,0.27f), FVector(-12.f,-32.f,37.f));
    EarR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EarR")); EarR->SetupAttachment(Head);
    ConfigureVisual(EarR, Sphere, FVector(0.24f,0.16f,0.27f), FVector(-12.f,32.f,37.f));

    EyeL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeL")); EyeL->SetupAttachment(Head);
    ConfigureVisual(EyeL, Sphere, FVector(0.075f), FVector(42.f,-24.f,12.f));
    EyeR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeR")); EyeR->SetupAttachment(Head);
    ConfigureVisual(EyeR, Sphere, FVector(0.075f), FVector(42.f,24.f,12.f));
    Nose = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Nose")); Nose->SetupAttachment(Head);
    ConfigureVisual(Nose, Sphere, FVector(0.07f,0.09f,0.065f), FVector(49.f,0.f,-3.f));

    PawL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawL")); PawL->SetupAttachment(RootComponent);
    ConfigureVisual(PawL, Sphere, FVector(0.12f,0.10f,0.18f), FVector(28.f,-34.f,3.f));
    PawR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawR")); PawR->SetupAttachment(RootComponent);
    ConfigureVisual(PawR, Sphere, FVector(0.12f,0.10f,0.18f), FVector(28.f,34.f,3.f));
    FootL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootL")); FootL->SetupAttachment(RootComponent);
    ConfigureVisual(FootL, Sphere, FVector(0.15f,0.12f,0.10f), FVector(8.f,-24.f,-50.f));
    FootR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootR")); FootR->SetupAttachment(RootComponent);
    ConfigureVisual(FootR, Sphere, FVector(0.15f,0.12f,0.10f), FVector(8.f,24.f,-50.f));

    Backpack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Backpack")); Backpack->SetupAttachment(RootComponent);
    ConfigureVisual(Backpack, Cube, FVector(0.20f,0.28f,0.28f), FVector(-43.f,0.f,12.f));
    Backpack->SetRelativeRotation(FRotator(0,0,8));

    CartRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CartRoot")); CartRoot->SetupAttachment(RootComponent);
    CartRoot->SetRelativeLocation(FVector(-108.f,0.f,-35.f));
    CartBed = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CartBed")); CartBed->SetupAttachment(CartRoot);
    ConfigureVisual(CartBed, Cube, FVector(0.48f,0.46f,0.13f), FVector(0,0,18.f));
    WheelL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelL")); WheelL->SetupAttachment(CartRoot);
    ConfigureVisual(WheelL, Cylinder, FVector(0.22f,0.22f,0.10f), FVector(0,-52.f,0));
    WheelL->SetRelativeRotation(FRotator(90,0,0));
    WheelR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelR")); WheelR->SetupAttachment(CartRoot);
    ConfigureVisual(WheelR, Cylinder, FVector(0.22f,0.22f,0.10f), FVector(0,52.f,0));
    WheelR->SetRelativeRotation(FRotator(90,0,0));
    CartCargo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CartCargo")); CartCargo->SetupAttachment(CartRoot);
    ConfigureVisual(CartCargo, Cube, FVector(0.25f,0.26f,0.24f), FVector(0,0,50.f));
    CartCargo->SetVisibility(false);
}

static void ApplyTint(UStaticMeshComponent* Comp, UMaterialInterface* Base, const FLinearColor& Color)
{
    if (!Comp || !Base) return;
    UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Base, Comp);
    if (!MID) return;
    MID->SetVectorParameterValue(TEXT("Color"), Color);
    Comp->SetMaterial(0, MID);
}

void AHEHamsterCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (GetActorLocation().Z < 80.f)
    {
        SetActorLocation(FVector(0.f, 0.f, 150.f));
    }

    UMaterialInterface* Base = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    const FLinearColor Golden(0.73f,0.39f,0.15f);
    const FLinearColor Cream(0.94f,0.76f,0.48f);
    const FLinearColor Dark(0.012f,0.010f,0.009f);
    const FLinearColor Pink(0.90f,0.40f,0.48f);
    ApplyTint(Body, Base, Golden); ApplyTint(Head, Base, Golden);
    ApplyTint(EarL, Base, FLinearColor(0.72f,0.30f,0.30f)); ApplyTint(EarR, Base, FLinearColor(0.72f,0.30f,0.30f));
    ApplyTint(Belly, Base, Cream); ApplyTint(PawL, Base, Cream); ApplyTint(PawR, Base, Cream); ApplyTint(FootL, Base, Cream); ApplyTint(FootR, Base, Cream);
    ApplyTint(EyeL, Base, Dark); ApplyTint(EyeR, Base, Dark); ApplyTint(Nose, Base, Pink);
    ApplyTint(Backpack, Base, FLinearColor(0.08f,0.34f,0.37f));
    ApplyTint(CartBed, Base, FLinearColor(0.43f,0.22f,0.08f));
    ApplyTint(WheelL, Base, FLinearColor(0.16f,0.08f,0.04f)); ApplyTint(WheelR, Base, FLinearColor(0.16f,0.08f,0.04f));
    ApplyTint(CartCargo, Base, FLinearColor(0.70f,0.47f,0.18f));
}

void AHEHamsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHEHamsterCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHEHamsterCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AHEHamsterCharacter::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AHEHamsterCharacter::StopSprint);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AHEHamsterCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("SaveGame"), IE_Pressed, this, &AHEHamsterCharacter::SaveGame);
    PlayerInputComponent->BindAction(TEXT("LoadGame"), IE_Pressed, this, &AHEHamsterCharacter::LoadGame);
}

void AHEHamsterCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(YawRot).GetUnitAxis(EAxis::X), Value);
}

void AHEHamsterCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y), Value);
}

void AHEHamsterCharacter::StartSprint() { GetCharacterMovement()->MaxWalkSpeed = 720.f; }
void AHEHamsterCharacter::StopSprint() { GetCharacterMovement()->MaxWalkSpeed = 420.f; }

void AHEHamsterCharacter::SetHasCargo(bool bNewHasCargo)
{
    bHasCargo = bNewHasCargo;
    if (CartCargo) CartCargo->SetVisibility(bHasCargo);
}

void AHEHamsterCharacter::Interact()
{
    if (bHasCargo) return;
    AHECargo* Closest = nullptr;
    float BestDistSq = FMath::Square(250.f);
    for (TActorIterator<AHECargo> It(GetWorld()); It; ++It)
    {
        if (It->IsPickedUp()) continue;
        const float D = FVector::DistSquared(GetActorLocation(), It->GetActorLocation());
        if (D < BestDistSq) { BestDistSq = D; Closest = *It; }
    }
    if (Closest)
    {
        Closest->Pickup();
        SetHasCargo(true);
        if (AHEHUD* HUD = Cast<AHEHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
            HUD->ShowMessage(TEXT("Cargo loaded! Deliver it to the glowing station."), 3.5f);
    }
}

void AHEHamsterCharacter::SaveGame()
{
    if (AHEGameModeBase* GM = Cast<AHEGameModeBase>(UGameplayStatics::GetGameMode(this))) GM->SaveProgress(this);
}

void AHEHamsterCharacter::LoadGame()
{
    if (AHEGameModeBase* GM = Cast<AHEGameModeBase>(UGameplayStatics::GetGameMode(this))) GM->LoadProgress(this);
}
