// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rosario_GAM415_FinalProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS(config = Game)
class ROSARIO_GAM415_FINAL_API ARosario_GAM415_FinalProjectile : public AActor
{
    GENERATED_BODY()

    /** Sphere collision component */
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComp;

    /** Projectile movement component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

    /** Projectile mesh */
    UPROPERTY(VisibleAnywhere, Category = Projectile)
    UStaticMeshComponent* BallMesh;

    /** Base decal material */
    UPROPERTY(EditAnywhere, Category = Decal)
    UMaterialInterface* baseMat;

    /** Base projectile material */
    UPROPERTY(EditAnywhere, Category = Projectile)
    UMaterialInterface* ProjectileMat;

    /** Dynamic projectile material instance */
    UPROPERTY()
    UMaterialInstanceDynamic* ProjectileMID;

    /** Stores the random projectile/decal color so both match */
    FVector4 ProjectileColor;

public:
    ARosario_GAM415_FinalProjectile();

    /** Called when projectile hits something */
    UFUNCTION()
    void OnHit(
        UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);

    /** Returns CollisionComp subobject **/
    USphereComponent* GetCollisionComp() const { return CollisionComp; }

    /** Returns ProjectileMovement subobject **/
    UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};