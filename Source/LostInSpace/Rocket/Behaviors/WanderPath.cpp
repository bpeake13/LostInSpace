// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Gameplay/BaseEnemyPawn.h"
#include "Rocket/Navigation/SpaceNavigationManager.h"
#include "WanderPath.h"


UWanderPath::UWanderPath()
{
	ReCalculateTime = 10.f;
	FixTime = 1.f;

	MaxWanderRange = 500000.f;
	MoveAcceleration = 5000.f;

	NodeChangeDistance = 1000.f;
}

void UWanderPath::Tick(UHierarchicalStateMachine* machine)
{
	UE_LOG(LogTemp, Log, TEXT("Wandering"));

	ABaseEnemyPawn* pawn = Cast<ABaseEnemyPawn>(machine->GetOwnerActor());
	if (!pawn)
		return;

	if (reCalcTimer <= 0 || path.Num() == 0)
	{
		FVector start = pawn->GetActorLocation();
		FVector end = FVector(FMath::FRandRange(-MaxWanderRange, MaxWanderRange), FMath::FRandRange(-MaxWanderRange, MaxWanderRange), 0);

		USpaceNavigationManager::GetBestPath(start, end, path);

		reCalcTimer = ReCalculateTime;
	}

	if (fixTimer <= 0)
	{
		fixTimer = FixTime;
	}

	reCalcTimer -= machine->GetDeltaTime();
	fixTimer -= machine->GetDeltaTime();

	if (path.Num() > 0)
	{
		//get the next nav point in the chain
		USpaceNavPoint* next = path[0];

		if (pawn->MoveTo(next->GetComponentLocation(), MoveAcceleration, NodeChangeDistance))
			path.RemoveAt(0);
	}

	FVector lastLocation = pawn->GetActorLocation();
	for (USpaceNavPoint* navPoint : path)
	{
		FVector current = navPoint->GetComponentLocation();

		DrawDebugLine(pawn->GetWorld(), lastLocation, current, FColor::Green, false, -1.f, 15, 50.f);

		lastLocation = current;
	}
}