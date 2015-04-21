// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "CanSeePlayerPlanet.h"





bool UCanSeePlayerPlanet::Check(UHierarchicalStateMachine* machine)
{
	ABaseEnemyPawn* enemyPawn = Cast<ABaseEnemyPawn>(machine->GetOwnerActor());
	checkf(enemyPawn, TEXT("Error: No enemy pawn for this condition"));

	return enemyPawn->CanSeePlayer();
}
