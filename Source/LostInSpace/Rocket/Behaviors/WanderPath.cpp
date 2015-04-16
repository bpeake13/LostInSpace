// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Gameplay/BaseEnemyPawn.h"
#include "Rocket/Navigation/SpaceNavigationManager.h"
#include "WanderPath.h"


UWanderPath::UWanderPath()
{
	ReCalculateTime = 10.f;
	FixTime = 0.1f;

	MaxWanderRange = 500000.f;
}

void UWanderPath::Tick(UHierarchicalStateMachine* machine)
{
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

		if (pawn->MoveTo(next->GetComponentLocation(), MoveForce, NodeChangeDistance))
			path.RemoveAt(0);
	}
}