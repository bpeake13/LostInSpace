// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PawnPlanatoidMovementComponent.h"

UPawnPlanatoidMovementComponent::UPawnPlanatoidMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	forceAccumulator = FVector::ZeroVector;
	accelerationAccumulator = FVector::ZeroVector;

	bHasGravity = true;
	bAlignToGravity = true;

	bForceDirty = false;

	facingDirection = 0.f;

	InputAcceleration = 50.f;
}

void UPawnPlanatoidMovementComponent::InitializeComponent()
{
	AActor* owner = GetOwner();

	PlanatoidData = ConstructObject<UPlanatoidDataComponent>(UPlanatoidDataComponent::StaticClass(), this, FName("PlanatoidData"));
	PlanatoidData->RegisterComponent();

	Super::InitializeComponent();
}

UPlanatoidDataComponent* UPawnPlanatoidMovementComponent::GetPlanatoidData() const
{
	return this->PlanatoidData;
}

void UPawnPlanatoidMovementComponent::AddForce(const FVector& force)
{
	bForceDirty = true;
	forceAccumulator += force;
}

void UPawnPlanatoidMovementComponent::AddAccleration(const FVector& acceleration)
{
	accelerationAccumulator += acceleration;
}

bool UPawnPlanatoidMovementComponent::ShouldTick() const
{
	return UpdatedPrimitive && GetWorld();
}

void UPawnPlanatoidMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (!ShouldTick() || ShouldSkipUpdate(DeltaTime))
	{
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		return;
	}

	ApplyGravity();

	ApplyForces();

	FVector inputVector = ConsumeInputVector();
	FVector relativeInputVector = PlanatoidData->GetOrientationMatrix().TransformVector(inputVector);
	AddAccleration(inputVector * InputAcceleration);

	FVector delta = CalculateMoveDelta(DeltaTime, Velocity);

	MoveComponent(delta, DeltaTime);
}

void UPawnPlanatoidMovementComponent::ApplyForces()
{
	if (!bForceDirty)
		return;

	float invMass = 1.f / UpdatedPrimitive->GetMass();
	AddAccleration(forceAccumulator * invMass);

	bForceDirty = false;
	forceAccumulator = FVector::ZeroVector;
}

void UPawnPlanatoidMovementComponent::ApplyGravity()
{
	if (!bHasGravity)
		return;

	UWorld* world = GetWorld();

	FVector up = PlanatoidData->GetUpVector();
	float gravity = world->GetGravityZ();

	AddAccleration(up * gravity);
}

FVector UPawnPlanatoidMovementComponent::LimitVelocity(FVector& inVelocity) const
{
	return inVelocity;
}

FVector UPawnPlanatoidMovementComponent::CalculateVelocity(float deltaTime, const FVector& inVelocity)
{
	FVector newVelocity = inVelocity + accelerationAccumulator * deltaTime;
	accelerationAccumulator = FVector::ZeroVector;

	newVelocity = LimitVelocity(newVelocity);

	return newVelocity;
}

FVector UPawnPlanatoidMovementComponent::CalculateMoveDelta(float deltaTime, const FVector& inVelocity)
{
	FVector newVelocity = CalculateVelocity(deltaTime, inVelocity);
	FVector deltaVelocity = newVelocity - inVelocity;

	return inVelocity * deltaTime + deltaVelocity * 0.5f * deltaTime;
}

void UPawnPlanatoidMovementComponent::MoveComponent(const FVector& delta, const float deltaTime)
{
	FRotator rotation = UpdatedComponent->GetComponentRotation();
	if (bAlignToGravity)
	{		
		FMatrix orientationMatrix = PlanatoidData->GetOrientationMatrix();
		rotation = orientationMatrix.Rotator();

		rotation.Yaw += facingDirection;
	}

	FVector startPos = UpdatedComponent->GetComponentLocation();

	FHitResult hit;
	SafeMoveUpdatedComponent(delta, rotation, true, hit);

	FVector endPos = UpdatedComponent->GetComponentLocation();

	Velocity = (endPos - startPos) * (1.f / deltaTime);
}
