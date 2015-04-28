// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PawnPlanatoidMovementComponent.h"
#include "FallingPlanatoidMovementMode.h"
#include "GroundPlanatoidMovementMode.h"



void UFallingPlanatoidMovementMode::EnterMode()
{
	
}

void UFallingPlanatoidMovementMode::ExitMode()
{
	
}

void UFallingPlanatoidMovementMode::IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn)
{
	outReturn.BaseComponent = NULL;
	outReturn.BaseBone = NAME_None;

	//Falling only allows for lateral input, so we get the acceleration that is lateral to the up plane
	FVector inputAccleration = FVector::VectorPlaneProject(tickParams.InputVector, tickParams.Up) * tickParams.Owner->GetAirControl();
	FVector newVelocity = CalculateVelocity(tickParams.Owner->GetRelativeVelocity(), tickParams.Acceleration + inputAccleration, tickParams.DeltaTime);
	
	float airFriction = tickParams.Owner->GetPhysicsVolume()->FluidFriction + tickParams.Owner->GetAirFriction();
	newVelocity -= newVelocity * airFriction * tickParams.TimeSlice;//modify the new velocity according to air friction

	//the magnitude of the falling velocity
	float fallingMagnitude = FVector::DotProduct(newVelocity, tickParams.Up);
	FVector fallingVelocity = tickParams.Up * FMath::Max(fallingMagnitude, -tickParams.Owner->GetMaxFallingSpeed());

	FVector tangentialVelocity = FVector::VectorPlaneProject(newVelocity, tickParams.Up).GetClampedToMaxSize(tickParams.Owner->GetMaxAirMovementSpeed());

	newVelocity = fallingVelocity + tangentialVelocity;

	FVector delta = CalculateDelta(tickParams.Owner->Velocity, newVelocity, tickParams.TimeSlice);

	FHitResult result;
	tickParams.Owner->MoveComponent(delta, false, result);

	outReturn.OutTime = tickParams.TimeSlice * result.Time;

	//if we do not have a hit then we are done calculating
	if (!result.IsValidBlockingHit())
	{
		outReturn.OutVelocity = newVelocity;
		return;
	}

	//signal a hit
	outReturn.bWasHit = true;

	if (tickParams.Owner->CanStand(result))
	{
		outReturn.OutNextMode = UGroundPlanatoidMovementMode::StaticClass();
	}
	else
	{
		//otherwise we will finish falling while sliding allong the surface
		float remainingTime = 1.f - result.Time;
		float slideTime = tickParams.Owner->SlideAlongSurface(delta, remainingTime, result.ImpactNormal, result, true);

		outReturn.OutTime += slideTime;
	}
}
