// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PawnPlanatoidMovementComponent.h"
#include "PawnPlanatoidMovementMode.h"

void UPawnPlanatoidMovementMode::IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn)
{
	
}

void UPawnPlanatoidMovementMode::EnterMode()
{

}

void UPawnPlanatoidMovementMode::ExitMode()
{

}

FVector UPawnPlanatoidMovementMode::CalculateDelta(const FTickParams& tickParams, float deltaTime) const
{
	FVector startVelocity = tickParams.Owner->Velocity;
	FVector endVelocity = tickParams.TargetVelocity;

	return startVelocity * deltaTime + (endVelocity - startVelocity) * 0.5f * deltaTime;
}
