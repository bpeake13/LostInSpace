// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "BaseEnemyPawn.h"

ABaseEnemyPawn::ABaseEnemyPawn()
{
	HSM = CreateDefaultSubobject<UHierarchicalStateMachine>("Hierarchical State Machine");
}

void ABaseEnemyPawn::BeginPlay()
{
	if (HSM)
		HSM->SetOwnerActor(this);
}

void ABaseEnemyPawn::Tick(float DeltaSeconds)
{
	if (HSM)
		HSM->Tick(DeltaSeconds);
}

bool ABaseEnemyPawn::MoveTo(const FVector& location, const float force, const float tolerance)
{
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	checkf(rootPrimitive, TEXT("FATAL ERROR: Root is not of type UPrimitive"));

	FVector offset = location - GetActorLocation();
	FVector direction = offset.GetSafeNormal();
	FVector forceVector = direction * force;

	FVector decceleration = -forceVector / rootPrimitive->GetMass();
	
	float time = (-GetVelocity() / decceleration).Size();
	float distance = (GetVelocity() * time + 0.5 * decceleration * time * time).Size();
	float distanceSquared = distance * distance;

	if (offset.SizeSquared() <= distanceSquared + distanceSquared * 0.01f)
		rootPrimitive->AddForce(decceleration);
	else
		rootPrimitive->AddForce(forceVector);

	return offset.SizeSquared() < tolerance * tolerance;
}
