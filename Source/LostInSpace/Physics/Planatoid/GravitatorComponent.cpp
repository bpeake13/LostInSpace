// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "GravitatorComponent.h"


// Sets default values for this component's properties
UGravitatorComponent::UGravitatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	GravityStrength = 980.f;
	// ...
}


void UGravitatorComponent::GetUpAtPoint(const FVector& point, FVector& o_upVector)
{
	o_upVector = FVector(0, 0, 1);
}

// Called when the game starts
void UGravitatorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	TArray<USceneComponent*> children;
	GetChildrenComponents(true, children);

	for (TArray<USceneComponent*>::TIterator i = children.CreateIterator(); i; i++)
	{
		USceneComponent* sc = *i;
		UPrimitiveComponent* pc = Cast<UPrimitiveComponent>(sc);
		if (!pc)
			continue;

		affectorZones.Add(pc);

		pc->bGenerateOverlapEvents = true;
		pc->bMultiBodyOverlap = false;
	}
}

void UGravitatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	for (UPrimitiveComponent* zonePrimitive : affectorZones)
	{
		TArray<AActor*> overlappingActors;
		zonePrimitive->GetOverlappingActors(overlappingActors);

		for (AActor* actor : overlappingActors)
		{
			UPlanatoidDataComponent* planatoidData = Cast<UPlanatoidDataComponent>(actor->GetComponentByClass(UPlanatoidDataComponent::StaticClass()));
			if (!planatoidData)
				continue;

			FVector up;
			GetUpAtPoint(actor->GetActorLocation(), up);

			planatoidData->SetUpVector(up);
		}

		overlappingActors.Empty();
	}
}