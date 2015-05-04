// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Rocket/PlayerPlanetPawn.h"
#include "LostInSpaceCharacterController.h"

UClass* ALostInSpaceCharacterController::GetDefaultPawnClass() const
{
	return DefaultPlanetPawn;
}

bool ALostInSpaceCharacterController::CanSpawnAtStartSpot() const
{
	APlayerStart* startPoint = Cast<APlayerStart>(StartSpot.Get());
	if (!startPoint)
		return false;

	if (startPoint->PlayerStartTag == TEXT("Player"))
		return true;

	return false;
}
