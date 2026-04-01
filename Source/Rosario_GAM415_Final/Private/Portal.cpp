// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Constructor
APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create portal mesh (visual surface)
    PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
    RootComponent = PortalMesh;

    // Create collision box (for teleport detection)
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);

    // Create scene capture (camera for portal view)
    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(RootComponent);

    bCanTeleport = true;
}

// BeginPlay
void APortal::BeginPlay()
{
    Super::BeginPlay();

    // Bind overlap event to teleport function
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
}

// Tick (called every frame)
void APortal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update portal camera each frame
    UpdatePortals();
}

// Teleport logic
void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!bCanTeleport || !LinkedPortal) return;

    if (OtherActor)
    {
        // Move player to linked portal
        FVector NewLocation = LinkedPortal->GetActorLocation() + LinkedPortal->GetActorForwardVector() * 100.f;

        // Rotate player to match portal direction
        FRotator NewRotation = LinkedPortal->GetActorRotation();

        OtherActor->SetActorLocation(NewLocation);
        OtherActor->SetActorRotation(NewRotation);

        // Prevent instant re-trigger
        bCanTeleport = false;

        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
            {
                bCanTeleport = true;
            });
    }
}

// Simple portal camera update
void APortal::UpdatePortals()
{
    if (!LinkedPortal || !SceneCapture) return;

    // Match camera to linked portal
    SceneCapture->SetWorldLocation(LinkedPortal->GetActorLocation());
    SceneCapture->SetWorldRotation(LinkedPortal->GetActorRotation());
}

// Optional (not required but matches your .h)
void APortal::SetBool(AFirstperson415Character* PlayerChar)
{
    // This function can be used later for advanced logic if needed
}