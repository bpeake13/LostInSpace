// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Rocket/Navigation/DestinationPoint.h"
#include "LostInSpaceCharacterController.h"

void ALostInSpaceCharacterController::AddScore(float deltaScore)
{
	score += deltaScore;
}

float ALostInSpaceCharacterController::GetScore() const
{
	return score;
}

void ALostInSpaceCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!destination.IsValid())
		ChangeToNextDestination();
}

void ALostInSpaceCharacterController::ChangeToNextDestination()
{
	UWorld* world = this->GetWorld();
	if (!world)
		return;

	for (TObjectIterator<UDestinationPoint> nextDestItr; nextDestItr; ++nextDestItr)
	{
		if (nextDestItr->GetWorld() != world)
			continue;

		destination = *nextDestItr;
		if (!destination.IsValid())
			continue;

		APlayerSpacePawn* spacePawn = Cast<APlayerSpacePawn>(this->GetPawn());
		if (!spacePawn)
			return;

		spacePawn->SetDestination(destination.Get());
		return;
	}

	APlayerSpacePawn* spacePawn = Cast<APlayerSpacePawn>(this->GetPawn());
	spacePawn->SetDestination(NULL);
}
