// Fill out your copyright notice in the Description page of Project Settings.
// This file implements procedural terrain generation using Unreal Engine's
// ProceduralMeshComponent. It demonstrates how graphics APIs can dynamically
// generate and modify geometry at runtime using vertices, triangles, and noise.

#include "ProceduralTerrain.h"

// Constructor: initializes the terrain actor
AProceduralTerrain::AProceduralTerrain()
{
    PrimaryActorTick.bCanEverTick = false;

    // ProceduralMeshComponent allows us to generate and render custom geometry at runtime
    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));

    // Set the procedural mesh as the root so it becomes the visible terrain
    RootComponent = ProcMesh;
}

// Called when the game starts
void AProceduralTerrain::BeginPlay()
{
    Super::BeginPlay();

    // Generate terrain when the game begins
    GenerateTerrain();
}

// Called whenever a property is changed in the editor
void AProceduralTerrain::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Regenerate terrain when XSize or YSize is modified in the editor
    GenerateTerrain();
}

// Function that builds the terrain mesh using vertices and triangles
void AProceduralTerrain::GenerateTerrain()
{
    // Clear previous vertex data to rebuild terrain
    Vertices.Empty();

    // Arrays required by the graphics API to define mesh geometry
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    // Loop through grid to generate vertices
    for (int32 X = 0; X <= XSize; X++)
    {
        for (int32 Y = 0; Y <= YSize; Y++)
        {
            // Perlin noise generates smooth natural terrain instead of random spikes
            float Noise = FMath::PerlinNoise2D(FVector2D(X * NoiseScale, Y * NoiseScale));

            // Height of terrain is controlled by noise value
            float Z = Noise * HeightScale;

            // Add vertex position in 3D space
            Vertices.Add(FVector(X * GridSpacing, Y * GridSpacing, Z));

            // Default normal pointing upward (used for lighting calculations)
            Normals.Add(FVector::UpVector);

            // UV coordinates used for texturing the mesh
            UVs.Add(FVector2D(X / (float)XSize, Y / (float)YSize));
            
            // Vertex color (can be used for visual effects or debugging)
            VertexColors.Add(FLinearColor::White);

            // Tangent controls how lighting interacts with the surface
            Tangents.Add(FProcMeshTangent(1, 0, 0));
        }
    }

    // Generate triangles to connect vertices into a mesh surface
    for (int32 X = 0; X < XSize; X++)
    {
        for (int32 Y = 0; Y < YSize; Y++)
        {
            // Calculate indices for square grid
            int32 BottomLeft = X * (YSize + 1) + Y;
            int32 BottomRight = BottomLeft + 1;
            int32 TopLeft = BottomLeft + (YSize + 1);
            int32 TopRight = TopLeft + 1;

            // First triangle of the quad
            Triangles.Add(BottomLeft);
            Triangles.Add(TopLeft);
            Triangles.Add(BottomRight);

            // Second triangle of the quad
            Triangles.Add(BottomRight);
            Triangles.Add(TopLeft);
            Triangles.Add(TopRight);
        }
    }

    // Send mesh data to Unreal Engine's rendering system
    // This is where the graphics API takes our data and draws it on screen
    ProcMesh->CreateMeshSection_LinearColor(
        0,
        Vertices,
        Triangles,
        Normals,
        UVs,
        VertexColors,
        Tangents,
        true
    );

   
    ProcMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

// Function that modifies terrain when player interacts with it
void AProceduralTerrain::ModifyTerrainAtPoint(FVector HitLocation, float Radius, float Strength)
{
    // Loop through all vertices
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        // Calculate 2D distance from hit location to vertex
        float Distance = FVector2D::Distance(
            FVector2D(Vertices[i].X, Vertices[i].Y),
            FVector2D(HitLocation.X, HitLocation.Y)
        );

        // If vertex is within the modification radius
        if (Distance < Radius)
        {
            // Adjust height (Z value) to modify terrain
            Vertices[i].Z += Strength;
        }
    }

    // Update mesh so changes appear visually in real time
    // This tells the graphics API to redraw the mesh with new vertex data
    ProcMesh->UpdateMeshSection_LinearColor(0, Vertices, {}, {}, {}, {});
}
