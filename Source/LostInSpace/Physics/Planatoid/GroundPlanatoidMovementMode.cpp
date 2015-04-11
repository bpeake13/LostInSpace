// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "FallingPlanatoidMovementMode.h"
#include "PawnPlanatoidMovementComponent.h"
#include "PawnPlanatoidMovementComponent.h"
#include "GroundPlanatoidMovementMode.h"


void UGroundPlanatoidMovementMode::EnterMode()
{
	lastGroundHit = FHitResult(NULL, NULL, FVector::ZeroVector, FVector::ZeroVector);
	UE_LOG(LogTemp, Log, TEXT("Grounded"));
}

void UGroundPlanatoidMovementMode::ExitMode()
{

}

void UGroundPlanatoidMovementMode::IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn)
{
	FHitResult hit;

	//if this is the first tick while on the ground then we want to find the ground
	if (!lastGroundHit.IsValidBlockingHit())
	{
		bool isGround = FindGround(tickParams, lastGroundHit);
		if (!isGround)
		{
			outReturn.OutNextMode = UFallingPlanatoidMovementMode::StaticClass();
			return;
		}
	}

	//get the delta along the ground plane
	FVector delta = CalculateDelta(tickParams, tickParams.TimeSlice);
	delta = FVector::VectorPlaneProject(delta, lastGroundHit.ImpactNormal);

	tickParams.Owner->MoveComponent(delta, hit);

	//if we can stand on the hit surface, then allow it and handle that surface next iteration
	if (!tickParams.Owner->CanStand(hit))
	{
		outReturn.OutTime = hit.Time * tickParams.TimeSlice;
		return;
	}

	if (!FindGround(tickParams, lastGroundHit))
	{
		outReturn.OutNextMode = UFallingPlanatoidMovementMode::StaticClass();
	}
	else
	{

	}
}

bool UGroundPlanatoidMovementMode::FindGround(const FTickParams& tickParams, FHitResult& floorResult)
{
	FScopedMovementUpdate scopeMove = FScopedMovementUpdate(tickParams.Owner->UpdatedComponent);

	//distance to step down is 1/20th the height of our character
	FVector stepDown = tickParams.Owner->UpdatedPrimitive->Bounds.GetSphere().W * 0.1f * -tickParams.Owner->GetUp();

	//move down by the step amount
	tickParams.Owner->MoveComponent(stepDown, floorResult);

	//if we do not have a hit then there is no ground, we should revert back and transition into a falling state
	if (!floorResult.IsValidBlockingHit())
	{
		scopeMove.RevertMove();
		return false;
	}

	//if we are on standable ground then we can quit
	if (tickParams.Owner->CanStand(floorResult))
		return true;

	//otherwise slide along the surface
	tickParams.Owner->SlideAlongSurface(stepDown, 1.f - floorResult.Time, floorResult.ImpactNormal, floorResult, false);

	//if we are on standable ground then we can quit
	if (tickParams.Owner->CanStand(floorResult))
		return true;

	//if we get nothing or hit something that is not a ground surface then just call out early
	scopeMove.RevertMove();
	return false;
}

bool UGroundPlanatoidMovementMode::MoveAlongGround(const FTickParams& tickParams)
{
	return false;
}
