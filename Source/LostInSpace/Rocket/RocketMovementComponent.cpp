// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "RocketMovementComponent.h"




URocketMovementComponent::URocketMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	forceAccumulator = FVector::ZeroVector;
	accelerationAccumulator = FVector::ZeroVector;

	PlaneConstraintOrigin = FVector::ZeroVector;
	PlaneConstraintNormal = FVector::UpVector;
	bConstrainToPlane = true;

	bForceDirty = false;
}

void URocketMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void URocketMovementComponent::AddForce(const FVector& force)
{
	bForceDirty = true;
	forceAccumulator += force;
}

void URocketMovementComponent::AddAccleration(const FVector& acceleration)
{
	accelerationAccumulator += acceleration;
}

bool URocketMovementComponent::ShouldTick() const
{
	return UpdatedPrimitive && GetWorld();
}

void URocketMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (!ShouldTick() || ShouldSkipUpdate(DeltaTime))
	{
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		return;
	}

	ApplyForces();

	FVector inputVector = ConsumeInputVector();
	AddAccleration(inputVector);

	FVector delta = CalculateMoveDelta(DeltaTime, Velocity);

	MoveComponent(delta, DeltaTime);
}

void URocketMovementComponent::ApplyForces()
{
	if (!bForceDirty)
		return;

	float invMass = 1.f / UpdatedPrimitive->GetMass();
	AddAccleration(forceAccumulator * invMass);

	bForceDirty = false;
	forceAccumulator = FVector::ZeroVector;
}

FVector URocketMovementComponent::LimitVelocity(FVector& inVelocity) const
{
	return ConstrainDirectionToPlane(inVelocity);
}

FVector URocketMovementComponent::CalculateVelocity(float deltaTime, const FVector& inVelocity)
{
	FVector newVelocity = inVelocity + accelerationAccumulator * deltaTime;
	accelerationAccumulator = FVector::ZeroVector;

	newVelocity = LimitVelocity(newVelocity);

	return newVelocity;
}

FVector URocketMovementComponent::CalculateMoveDelta(float deltaTime, const FVector& inVelocity)
{
	FVector newVelocity = CalculateVelocity(deltaTime, inVelocity);
	FVector deltaVelocity = newVelocity - inVelocity;

	return inVelocity * deltaTime + deltaVelocity * 0.5f * deltaTime;
}

void URocketMovementComponent::MoveComponent(const FVector& delta, const float deltaTime)
{
	FRotator rotation = UpdatedComponent->GetComponentRotation();

	FVector startPos = UpdatedComponent->GetComponentLocation();

	FHitResult hit;
	SafeMoveUpdatedComponent(delta, rotation, true, hit);

	FVector endPos = UpdatedComponent->GetComponentLocation();

	Velocity = (endPos - startPos) * (1.f / deltaTime);
}