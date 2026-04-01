// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ACubeDMIMod::ACubeDMIMod()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create box collision component
    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    RootComponent = boxComp;

    // Create cube mesh component
    cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    cubeMesh->SetupAttachment(boxComp);
}

void ACubeDMIMod::BeginPlay()
{
    Super::BeginPlay();

    boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

    if (baseMat)
    {
        dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);
    }

    if (cubeMesh && dmiMat)
    {
        cubeMesh->SetMaterial(0, dmiMat);
    }
}

void ACubeDMIMod::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACubeDMIMod::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && dmiMat)
    {
        float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
        float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
        float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

        FLinearColor randColor(ranNumX, ranNumY, ranNumZ, 1.f);

        dmiMat->SetVectorParameterValue("Color", randColor);
        dmiMat->SetScalarParameterValue("Darkness", ranNumX);

        UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            colorP,
            cubeMesh,
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset,
            true
        );
    }
}