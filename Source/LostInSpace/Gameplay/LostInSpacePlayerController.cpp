// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "LostInSpacePlayerController.h"





UClass* ALostInSpacePlayerController::GetDefaultPawnClass() const
{
	return NULL;
}

bool ALostInSpacePlayerController::CanSpawnAtStartSpot() const
{
	AActor* actorStartPoint = StartSpot.Get();
	return actorStartPoint != NULL;
}
