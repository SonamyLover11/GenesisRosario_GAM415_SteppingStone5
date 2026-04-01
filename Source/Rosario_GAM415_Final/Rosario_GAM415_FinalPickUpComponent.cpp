// Copyright Epic Games, Inc. All Rights Reserved.

#include "Rosario_GAM415_FinalPickUpComponent.h"

URosario_GAM415_FinalPickUpComponent::URosario_GAM415_FinalPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void URosario_GAM415_FinalPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &URosario_GAM415_FinalPickUpComponent::OnSphereBeginOverlap);
}

void URosario_GAM415_FinalPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ARosario_GAM415_FinalCharacter* Character = Cast<ARosario_GAM415_FinalCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
