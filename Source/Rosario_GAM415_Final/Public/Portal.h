// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Portal.generated.h"

class Rosario_GAM415_Final;

UCLASS()
class FIRSTPERSON415_API APortal; : public AActor
{
    GENERATED_BODY()

public:
    APortal();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Portal helper functions
    UFUNCTION()
    void SetBool(AFirstperson415Character* PlayerChar);

    UFUNCTION()
    void UpdatePortals();

    // Portal mesh (what you see)
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* PortalMesh;

    // Collision box for teleport
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionBox;

    // Scene capture for portal view
    UPROPERTY(VisibleAnywhere)
    USceneCaptureComponent2D* SceneCapture;

    // Render target for portal
    UPROPERTY(EditAnywhere)
    UTextureRenderTarget2D* RenderTarget;

    // Linked portal
    UPROPERTY(EditAnywhere)
    APortal* LinkedPortal;

    // Teleport cooldown
    bool bCanTeleport;

    // Overlap function
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};