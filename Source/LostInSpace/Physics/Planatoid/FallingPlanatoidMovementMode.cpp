// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PawnPlanatoidMovementComponent.h"
#include "FallingPlanatoidMovementMode.h"





void UFallingPlanatoidMovementMode::EnterMode()
{
	
}

void UFallingPlanatoidMovementMode::ExitMode()
{
	
}

void UFallingPlanatoidMovementMode::IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn)
{
	FVector delta = CalculateDelta(tickParams, tickParams.TimeSlice);
	
	FHitResult result;
	tickParams.Owner->MoveComponent(delta, result);

	outReturn.OutTime = tickParams.TimeSlice * result.Time;

	//if we do not have a hit then we are done calculating
	if (!result.IsValidBlockingHit())
		return;

	FVector up = tickParams.Owner->GetUp();

	float surfaceSlope = FVector::DotProduct(result.ImpactNormal, up);

	if (surfaceSlope <= tickParams.Owner->GetMaxGroundSlope())
	{
		//if we are less than the max slope then we need to become grounded
	}
	else
	{
		//otherwise we will finish falling while sliding allong the surface
		float remainingTime = 1.f - result.Time;
		float slideTime = tickParams.Owner->SlideAlongSurface(delta, remainingTime, result.ImpactNormal, result, true);

		outReturn.OutTime += slideTime;
	}
}
