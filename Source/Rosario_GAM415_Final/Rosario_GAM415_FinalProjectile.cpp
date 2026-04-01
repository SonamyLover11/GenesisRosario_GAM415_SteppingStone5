// Copyright Epic Games, Inc. All Rights Reserved.

#include "Rosario_GAM415_FinalProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ARosario_GAM415_FinalProjectile::ARosario_GAM415_FinalProjectile()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &ARosario_GAM415_FinalProjectile::OnHit);

    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    // Create projectile mesh in C++
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Use ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    // Randomize projectile color when spawned
    float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
    float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
    float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
    ProjectileColor = FVector4(ranNumX, ranNumY, ranNumZ, 1.f);

    // Create dynamic material instance for the projectile mesh
    ProjectileMID = nullptr;
    if (ProjectileMat)
    {
        BallMesh->SetMaterial(0, ProjectileMat);
        ProjectileMID = BallMesh->CreateDynamicMaterialInstance(0);

        if (ProjectileMID)
        {
            // Match this parameter name to the material parameter in Unreal
            ProjectileMID->SetVectorParameterValue(TEXT("ProjColor"), ProjectileColor);
        }
    }

    // Die after 3 seconds by default
    InitialLifeSpan = 3.0f;
}

void ARosario_GAM415_FinalProjectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
    {
        // Add impulse if we hit a physics object
        if (OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        }

        // Randomize decal splat frame
        float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

        // Spawn decal on impact
        auto Decal = UGameplayStatics::SpawnDecalAtLocation(
            GetWorld(),
            baseMat,
            FVector(100.f, 100.f, 100.f),
            Hit.Location,
            Hit.Normal.Rotation(),
            0.f
        );

        if (Decal)
        {
            auto MatInstance = Decal->CreateDynamicMaterialInstance();

            if (MatInstance)
            {
                // Use the same color as the projectile so they match
                MatInstance->SetVectorParameterValue(TEXT("ProjColor"), ProjectileColor);
                MatInstance->SetScalarParameterValue(TEXT("Frame"), frameNum);
            }
        }

        Destroy();
    }
}