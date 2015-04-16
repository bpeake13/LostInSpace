// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SpaceNavigationManager.h"
#include "UnrealEd.h"
#include "SpaceNavPoint.h"


// Sets default values for this component's properties
USpaceNavPoint::USpaceNavPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;

	bDirty = true;

	Radius = 500.f;
}


// Called when the game starts
void USpaceNavPoint::InitializeComponent()
{
#if WITH_EDITOR
	if (GIsEditor && FApp::IsGame())
		USpaceNavigationManager::AddNavPoint(this);
#else
	USpaceNavigationManager::AddNavPoint(this);
#endif // WITH_EDITOR

	Super::InitializeComponent();
}

void USpaceNavPoint::OnComponentCreated()
{
	Super::OnComponentCreated();
}

void USpaceNavPoint::Build()
{
	UWorld* world = GetWorld();
	if (!world || !bDirty)
		return;

	FVector location = GetComponentLocation();

	TArray<USpaceNavPoint*> consideringPoints;
	USpaceNavigationManager::NavPointsInRange(location, Radius, consideringPoints);

	FCollisionQueryParams queryParams;
	queryParams.bReturnFaceIndex = false;
	queryParams.bReturnPhysicalMaterial = false;
	FCollisionObjectQueryParams objParams;
	objParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	for (USpaceNavPoint* navPoint : consideringPoints)
	{
		if (this == navPoint)
			continue;

		FVector end = navPoint->GetComponentLocation();

		FHitResult result;
		if(world->LineTraceSingle(result, location, end, queryParams, objParams))
			continue;

		FSpaceNavEdge edge;
		edge.Target = navPoint;
		edge.Weight = (end - location).Size();

		Edges.Add(edge);
	}

	bDirty = false;
}