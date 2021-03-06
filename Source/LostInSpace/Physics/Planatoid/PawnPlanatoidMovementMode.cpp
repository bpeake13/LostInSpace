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

FVector UPawnPlanatoidMovementMode::CalculateDelta(const FVector& startVelocity, FVector& endVelocity, float deltaTime) const
{
	return startVelocity * deltaTime + (endVelocity - startVelocity) * 0.5f * deltaTime;
}

FVector UPawnPlanatoidMovementMode::CalculateVelocity(const FVector& inVelocity, const FVector& inAcceleration, const float deltaTime) const
{
	FVector newVelocity = inVelocity + inAcceleration * deltaTime;

	return newVelocity;
}
