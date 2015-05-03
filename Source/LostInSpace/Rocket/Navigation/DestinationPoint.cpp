// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "DestinationPoint.h"


// Sets default values for this component's properties
UDestinationPoint::UDestinationPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UDestinationPoint::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	
}

UDestinationPoint* UDestinationPoint::GetNext()
{
	UWorld* world = GetWorld();
	if (!world)
		return NULL;

	for (TObjectIterator<UDestinationPoint> nextItr; nextItr; ++nextItr)
	{
		if (nextItr->GetWorld() != world || nextItr->isDone || nextItr->Priority > Priority)
			continue;

		return *nextItr;
	}

	return NULL;
}
