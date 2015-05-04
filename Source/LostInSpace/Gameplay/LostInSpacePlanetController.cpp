// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Rocket/Navigation/DestinationPoint.h"
#include "GameFramework/PlayerStart.h"
#include "LostInSpacePlanetController.h"

UClass* ALostInSpacePlanetController::GetDefaultPawnClass() const
{
	return DefaultPawn;
}

void ALostInSpacePlanetController::AddScore(float deltaScore)
{
	score += deltaScore;
}

float ALostInSpacePlanetController::GetScore() const
{
	return score;
}

bool ALostInSpacePlanetController::CanSpawnAtStartSpot() const
{
	APlayerStart* startPoint = Cast<APlayerStart>(StartSpot.Get());
	if (!startPoint)
		return false;

	if (startPoint->PlayerStartTag == TEXT("Planet"))
		return true;

	return false;
}
