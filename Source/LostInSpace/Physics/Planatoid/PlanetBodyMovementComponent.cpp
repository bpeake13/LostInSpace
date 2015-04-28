// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PlanetBodyMovementComponent.h"

UPlanetBodyMovementComponent::UPlanetBodyMovementComponent(const FObjectInitializer& init)
	: Super(init)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_DuringPhysics;

	MaxSimulationSteps = 1;
	MaxSpeed = 2000.f;

	bForceDirty = false;

	LinearFriction = 10.f;
	BreakingForce = 1000.f;
}

void UPlanetBodyMovementComponent::AddForce(const FVector& forceVector)
{
	bForceDirty = true;
	forceAccumulator += forceVector;
}

void UPlanetBodyMovementComponent::AddAcceleration(const FVector& accelerationVector)
{
	accelerationAccumulator += accelerationVector;
}

void UPlanetBodyMovementComponent::ApplyForces()
{
	if (!bForceDirty)
		return;

	FVector accelerationVector = forceAccumulator * (1.f / UpdatedPrimitive->GetMass());
	AddAcceleration(accelerationVector);

	forceAccumulator = FVector::ZeroVector;

	bForceDirty = false;
}

FVector UPlanetBodyMovementComponent::CalculateVelocity(float deltaTime)
{
	FVector newVelocity = Velocity + accelerationAccumulator * deltaTime;
	LimitVelocity(newVelocity);

	FVector frictionVector = Velocity.GetSafeNormal() * (LinearFriction * deltaTime);
	return newVelocity - frictionVector;
}

FVector UPlanetBodyMovementComponent::CalculateDelta(const FVector& startVelocity, const FVector& endVelocity, float deltaTime)
{
	FVector delta = startVelocity * deltaTime + (endVelocity - startVelocity) * deltaTime * 0.5f;

	return delta;
}

void UPlanetBodyMovementComponent::LimitVelocity(FVector& velocityVector)
{
	velocityVector = velocityVector.GetClampedToMaxSize(MaxSpeed);
}

FVector UPlanetBodyMovementComponent::ApplyBreaking(const FVector& velocity, float deltaTime)
{
	FVector velocityDirection = velocity.GetSafeNormal();
	FVector decceleration = -velocityDirection * BreakingForce;

	FVector newVelocity = velocity + decceleration * deltaTime;

	//if we are near zero, or have changed directions, then we should make the new velocity zero
	if (newVelocity.IsNearlyZero())
		newVelocity = FVector::ZeroVector;
	else if ((newVelocity | velocityDirection) < 0)
		newVelocity = FVector::ZeroVector;

	return newVelocity;
}

void UPlanetBodyMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	FScopedMovementUpdate scopedMove = FScopedMovementUpdate(UpdatedPrimitive);

	float timeLeft = DeltaTime;
	
	int step = 0;
	float timeSlice = DeltaTime / MaxSimulationSteps;
	float lastSimulationTime = 0.f;
	while (step < MaxSimulationSteps && timeLeft >= 0.f)
	{
		float simulationTime = FMath::Max(timeSlice - lastSimulationTime, 0.f);

		lastSimulationTime = SimulateTick(simulationTime);

		timeLeft -= lastSimulationTime;
		if (timeLeft < -SMALL_NUMBER)
			UE_LOG(LogTemp, Warning, TEXT("Actor %s stepped %f seconds over simulation time."), *GetOwner()->GetPathName(), -timeLeft);

		step++;
	}

	accelerationAccumulator = FVector::ZeroVector;
}

float UPlanetBodyMovementComponent::SimulateTick(float deltaTime)
{
	FVector startLocation = UpdatedPrimitive->GetComponentLocation();

	//calculate our new velocity after this time tick
	FVector newVelocity = CalculateVelocity(deltaTime);

	//calculate the breaking on the new velocity if we have no outside acceleration, or if we are being signaled to do so
	if (bBreaking || accelerationAccumulator.IsNearlyZero())
		newVelocity = ApplyBreaking(newVelocity, deltaTime);

	FVector delta = CalculateDelta(Velocity, newVelocity, deltaTime);

	FHitResult hit;
	MoveUpdatedComponent(delta, UpdatedComponent->GetComponentRotation(), true, &hit);

	//if we are penetrating then we need to resolve this
	if (hit.bStartPenetrating)
	{
		FVector resolveVector = hit.ImpactNormal * hit.PenetrationDepth;
		MoveUpdatedComponent(resolveVector, UpdatedComponent->GetComponentRotation(), false);
	}

	//if we had a valid blocking hit then we should re-calculate our velocity
	if (hit.IsValidBlockingHit())
		Velocity = (UpdatedPrimitive->GetComponentLocation() - startLocation) * (1.f / deltaTime);
	else
		Velocity = newVelocity;

	return hit.Time * deltaTime;
}