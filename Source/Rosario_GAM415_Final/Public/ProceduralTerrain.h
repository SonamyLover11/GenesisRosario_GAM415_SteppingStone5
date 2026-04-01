// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrain.generated.h"

// This class generates terrain dynamically using a Procedural Mesh Component
// instead of a pre-made static mesh. It allows real-time updates to geometry.
UCLASS()
class ROSARIO_GAM415_FINAL_API AProceduralTerrain : public AActor
{
    GENERATED_BODY()

public:

    // Constructor initializes the procedural mesh component
    AProceduralTerrain();

protected:

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called when properties are changed in the editor (real-time updates)
    // This is important for making terrain dynamic with XSize/YSize
    virtual void OnConstruction(const FTransform& Transform) override;

public:

    // Number of vertices in X direction (controls terrain width)
    UPROPERTY(EditAnywhere, Category = "Terrain")
    int32 XSize = 50;

    // Number of vertices in Y direction (controls terrain length)
    UPROPERTY(EditAnywhere, Category = "Terrain")
    int32 YSize = 50;

    // Distance between each vertex (controls spacing of grid)
    UPROPERTY(EditAnywhere, Category = "Terrain")
    float GridSpacing = 100.f;

    // Controls how tall the terrain gets (Z height multiplier)
    UPROPERTY(EditAnywhere, Category = "Terrain")
    float HeightScale = 50.f;

    // Controls frequency of Perlin noise (terrain detail level)
    UPROPERTY(EditAnywhere, Category = "Terrain")
    float NoiseScale = 0.1f;

public:

    // Procedural mesh component used to generate terrain
    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* ProcMesh;

    // Stores vertex positions so they can be modified later (for raytrace interaction)
    TArray<FVector> Vertices;

    // Generates the terrain mesh using vertices and triangles
    void GenerateTerrain();

    // Modifies terrain when player interacts via raytrace
    void ModifyTerrainAtPoint(FVector HitLocation, float Radius, float Strength);

};