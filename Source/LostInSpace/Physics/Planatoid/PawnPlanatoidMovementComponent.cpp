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

	MaxSimulationSteps = 8;
	MaxGroundAngle = 45.f;

	MaxFallSpeed = 3000.f;
	MaxAirMovementSpeed = 7500.f;
	AirControl = 300.f;
	AirFriction = 0.1f;

	GroundAcceleration = 4000.f;
	MaxGroundSpeed = 300.f;
	BrakingDecceleration = 5000.f;
	GroundFriction = 8.f;

	currentMode = NULL;
}

void UPawnPlanatoidMovementComponent::InitializeComponent()
{
	MaxGroundAngle = FMath::Clamp(MaxGroundAngle, 0.f, 90.f);
	groundSlope = FMath::Cos(FMath::DegreesToRadians(MaxGroundAngle));

	AActor* owner = GetOwner();

	if (!PlanatoidData)
	{
		PlanatoidData = ConstructObject<UPlanatoidDataComponent>(UPlanatoidDataComponent::StaticClass(), this, FName("PlanatoidData"));
		PlanatoidData->RegisterComponent();
	}

	for (UClass* modeType : MovementModes)
	{
		if (!modeType)
			break;

		UPawnPlanatoidMovementMode* newMode = ConstructObject<UPawnPlanatoidMovementMode>(modeType, this);
		modeMap.Add(modeType, newMode);
		if (!currentMode)
		{
			currentMode = newMode;
			currentMode->EnterMode();
		}
	}
	Super::InitializeComponent();
}

void UPawnPlanatoidMovementComponent::PostLoad()
{

	Super::PostLoad();
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
	return PlanatoidData && UpdatedPrimitive && GetWorld() && currentMode;
}

FVector UPawnPlanatoidMovementComponent::GetUp() const
{
	if (PlanatoidData)
		return PlanatoidData->GetUpVector();
	return FVector(0, 0, 1);
}

FRotator UPawnPlanatoidMovementComponent::GetOrientation() const
{
	if (PlanatoidData)
		return PlanatoidData->GetOrientationMatrix().Rotator();
	return FRotator::ZeroRotator;
}

float UPawnPlanatoidMovementComponent::GetMaxGroundSlope() const
{
	return groundSlope;
}

bool UPawnPlanatoidMovementComponent::CanStand(FHitResult groundHit) const
{
	return groundHit.IsValidBlockingHit()
		&& !groundHit.bStartPenetrating
		&& groundHit.GetComponent()->CanCharacterStepUp(GetPawnOwner())
		&& FVector::DotProduct(groundHit.ImpactNormal, PlanatoidData->GetUpVector()) >= groundSlope;
}

void UPawnPlanatoidMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	FVector inputVector = ConsumeInputVector();//prevent input build up

	if (!ShouldTick() || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	ApplyGravity();

	ApplyForces();

	/*Package our tick params for this tick*/
	FTickParams tickParams;
	tickParams.DeltaTime = DeltaTime;
	tickParams.Owner = this;
	tickParams.OwnerPawn = GetPawnOwner();
	tickParams.UpdatedComponent = UpdatedComponent;
	tickParams.InputVector = inputVector;
	tickParams.Acceleration = accelerationAccumulator;
	tickParams.Up = GetUp();

	int32 iteration = 0;
	float timeLeft = DeltaTime;
	const float timeSlice = DeltaTime / (float)MaxSimulationSteps;
	float lastTimeRemaining = 0.f;
	while (iteration < MaxSimulationSteps && timeLeft >= 0.f)
	{
		FVector startPosition = UpdatedComponent->GetComponentLocation();

		tickParams.Iteration = iteration;
		tickParams.TimeSlice = timeSlice + lastTimeRemaining;

		//package the default return values
		FTickReturn returnValue;
		returnValue.OutIteration = iteration;
		returnValue.OutTime = tickParams.TimeSlice;
		returnValue.OutNextMode = NULL;
		returnValue.bWasHit = false;

		//iterate the physics on the current mode
		currentMode->IteratePhysics(tickParams, returnValue);

		//get the amount of time that was left, do not let this be below zero
		lastTimeRemaining = FMath::Max(timeSlice - returnValue.OutTime, 0.f);

		iteration = returnValue.OutIteration;
		timeLeft -= returnValue.OutTime;
		UClass* nextMode = returnValue.OutNextMode;

		//if we have a new mode then we need to swap it
		if (nextMode)
		{
			UPawnPlanatoidMovementMode** newMode = modeMap.Find(nextMode);
			if (newMode)
			{
				currentMode->ExitMode();
				currentMode = *newMode;
				currentMode->EnterMode();
			}
		}

		//if the time left has gone beyond the tick time then show a warning
		if (timeLeft < -SMALL_NUMBER)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s stepped more than delta time in a single step"), *GetFullName());
		}

		FVector endPosition = UpdatedComponent->GetComponentLocation();

		//if we were 
		if (returnValue.bWasHit && returnValue.OutTime != 0)
			Velocity = (endPosition - startPosition) * (1.f / returnValue.OutTime);
		else
			Velocity = returnValue.OutVelocity;

		iteration++;
	}

	accelerationAccumulator = FVector::ZeroVector;
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

FVector UPawnPlanatoidMovementComponent::CalculateVelocity(float deltaTime, const FVector& inVelocity)
{
	return FVector::ZeroVector;
}

void UPawnPlanatoidMovementComponent::MoveComponent(const FVector& delta, bool fastMove, FHitResult& outHit)
{
	FRotator rotation = UpdatedComponent->GetComponentRotation();
	if (bAlignToGravity)
	{	
		//during a fast move we do not need to compute the change in rotation for the new point
		if (!fastMove)
		{
			FVector newPosition = UpdatedComponent->GetComponentLocation() + delta;
			PlanatoidData->SetAtPoint(newPosition);
		}

		FMatrix orientationMatrix = PlanatoidData->GetOrientationMatrix();
		rotation = orientationMatrix.Rotator();

		rotation.Yaw += facingDirection;
	}

	MoveUpdatedComponent(delta, rotation, true, &outHit);
	if (outHit.bStartPenetrating)//resolve penetration
	{
		FVector resolveDelta = outHit.ImpactNormal * outHit.PenetrationDepth;
		MoveUpdatedComponent(resolveDelta, rotation, false);
	}
}