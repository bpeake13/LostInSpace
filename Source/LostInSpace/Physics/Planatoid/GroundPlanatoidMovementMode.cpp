// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "FallingPlanatoidMovementMode.h"
#include "PawnPlanatoidMovementComponent.h"
#include "PawnPlanatoidMovementComponent.h"
#include "GroundPlanatoidMovementMode.h"


void UGroundPlanatoidMovementMode::EnterMode()
{
	lastGroundHit = FHitResult(NULL, NULL, FVector::ZeroVector, FVector::ZeroVector);
}

void UGroundPlanatoidMovementMode::ExitMode()
{

}

/*	
	First we check to see if outside factors are causing us to be ungrounded
	This can be caused by velocity such as when jumping, or forces such as
	a tractor force. If we have a positive velocity after acceleration is applied
	then we will go back to the falling state
	
	Second we apply tangental movement either till we get a hit, or until the
	move is complete. If we have a hit, then we will check to see if it is ground.
	If it is ground then we can stop early. If it is not then we continue.
*/
void UGroundPlanatoidMovementMode::IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn)
{
	//this seems to cause better results
	outReturn.bWasHit = true;

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

	FVector delta;
	bool couldMoveOnGround = MoveAlongGround(tickParams, hit, delta);

	//If we couldn't move along the ground then attempt to figure out what the condition was
	if (!couldMoveOnGround)
	{
		bool canStep = TryStepUp(tickParams, hit, delta, hit);
		if (!canStep)
		{

		}
	}

	//find ground, if we do not have ground start falling
	bool foundGround = FindGround(tickParams, lastGroundHit);
	if (!foundGround)
	{
		outReturn.OutNextMode = UFallingPlanatoidMovementMode::StaticClass();
		return;
	}
}

bool UGroundPlanatoidMovementMode::FindGround(const FTickParams& tickParams, FHitResult& floorResult)
{
	FHitResult result;
	FScopedMovementUpdate scopeMove = FScopedMovementUpdate(tickParams.Owner->UpdatedComponent);

	//distance to step down is 1/20th the height of our character
	FVector stepDown = tickParams.Owner->UpdatedPrimitive->Bounds.GetSphere().W * 0.1f * -tickParams.Owner->GetUp();

	//move down by the step amount
	tickParams.Owner->MoveComponent(stepDown, true, result);

	//if we do not have a hit then there is no ground, we should revert back and transition into a falling state
	if (!result.IsValidBlockingHit())
	{
		scopeMove.RevertMove();
		return false;
	}

	//if we are on standable ground then we can quit
	if (tickParams.Owner->CanStand(result))
	{
		floorResult = result;
		return true;
	}

	//otherwise slide along the surface
	tickParams.Owner->SlideAlongSurface(stepDown, 1.f - floorResult.Time, floorResult.ImpactNormal, result, false);

	//if we are on standable ground then we can quit
	/*if (tickParams.Owner->CanStand(result))
	{
		floorResult = result;
		return true;
	}*/

	//if we get nothing or hit something that is not a ground surface then just call out early
	scopeMove.RevertMove();
	return false;
}

bool UGroundPlanatoidMovementMode::MoveAlongGround(const FTickParams& tickParams, FHitResult& hit, FVector& moveDelta)
{
	//create the scaled input acceleration that will travel along the ground
	FVector inputAcceleration = tickParams.InputVector * tickParams.Owner->GetGroundAccleration();
	inputAcceleration = FVector::VectorPlaneProject(inputAcceleration, lastGroundHit.ImpactNormal);

	//the total acceleration will be the acceleration caused by the world + that caused by input along the ground
	FVector totalAcceleration = inputAcceleration + tickParams.Acceleration;

	FVector newVelocity = CalculateVelocity(tickParams.Owner->Velocity, totalAcceleration, tickParams.TimeSlice);
	newVelocity = newVelocity.GetClampedToMaxSize(tickParams.Owner->GetMaxGroundSpeed());

	FVector velocityDirection = newVelocity.GetSafeNormal();
	newVelocity -= velocityDirection * -tickParams.Owner->GetGroundFriction() * tickParams.TimeSlice;

	//If we have nearly no input and velocity, then apply breaking
	if (inputAcceleration.IsNearlyZero() && !newVelocity.IsNearlyZero())
	{
		FVector decceleration = velocityDirection * (-tickParams.Owner->GetBrakingDecceleration() - tickParams.Owner->GetGroundFriction());
		newVelocity += decceleration * tickParams.TimeSlice;

		if (newVelocity.IsNearlyZero())
			newVelocity = FVector::ZeroVector;
		else if ((newVelocity | velocityDirection) < 0)
			newVelocity = FVector::ZeroVector;
	}

	moveDelta = CalculateDelta(tickParams.Owner->Velocity, newVelocity, tickParams.TimeSlice);
	moveDelta = FVector::VectorPlaneProject(moveDelta, lastGroundHit.ImpactNormal);

	//move the component by its tangental movement
	tickParams.Owner->MoveComponent(moveDelta, false, hit);

	if (hit.Time < 1.f)
		UE_LOG(LogTemp, Log, TEXT("%f"), hit.Time);

	if (hit.IsValidBlockingHit())
	{
		if (tickParams.Owner->CanStand(hit))
			lastGroundHit = hit;
		else
			return false;
	}

	return true;
}

bool UGroundPlanatoidMovementMode::TryStepUp(const FTickParams& tickParams, const FHitResult& stepHit, const FVector& delta, FHitResult& hitResult)
{
	//calculate the percent of the move we have left
	float moveLeft = 1.f - stepHit.Time;
	if (moveLeft <= KINDA_SMALL_NUMBER)//only try and step if a decent amount of time is left with the step
		return false;

	//scope the movement so that on fail we can undo it
	FScopedMovementUpdate stepScope = FScopedMovementUpdate(tickParams.Owner->UpdatedComponent);

	//for now step up the same distance that we use to find the floor
	FVector stepUpVector = tickParams.Owner->UpdatedPrimitive->Bounds.GetSphere().W * 0.1f * tickParams.Owner->GetUp();

	//Move our component up
	tickParams.Owner->MoveComponent(stepUpVector, true, hitResult);

	//if there is something above us then we should not step up
	if (hitResult.IsValidBlockingHit() && FVector::DotProduct(hitResult.ImpactNormal, stepUpVector) < 0.f)
	{
		stepScope.RevertMove();
		return false;
	}
	
	FVector deltaDirection = delta.GetSafeNormal();
	FVector deltaLeft = delta * moveLeft + deltaDirection * tickParams.Owner->UpdatedPrimitive->Bounds.GetSphere().W * 0.01f;

	tickParams.Owner->MoveComponent(deltaLeft, false, hitResult);
	
	if (tickParams.Owner->CanStand(hitResult))
	{
		//if we hit new ground then apply it and return true to signal that we are finished with a good step
		lastGroundHit = hitResult;
		return true;
	}
	else if (hitResult.Time <= KINDA_SMALL_NUMBER)
	{
		//if we bairly moved
		stepScope.RevertMove();
		return false;
	}
	else if (FindGround(tickParams, lastGroundHit))
	{
		return true;
	}
	else
	{
		stepScope.RevertMove();
		return false;
	}
}
