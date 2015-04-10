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

	MaxSimulationSteps = 8;
	MaxGroundAngle = 45.f;

	currentMode = NULL;
}

void UPawnPlanatoidMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPawnPlanatoidMovementComponent::PostLoad()
{
	MaxGroundAngle = FMath::Clamp(MaxGroundAngle, 0.f, 90.f);
	maxGroundSlope = FMath::Cos(FMath::DegreesToRadians(MaxGroundAngle));

	AActor* owner = GetOwner();

	PlanatoidData = ConstructObject<UPlanatoidDataComponent>(UPlanatoidDataComponent::StaticClass(), this, FName("PlanatoidData"));
	PlanatoidData->RegisterComponent();

	for (UClass* modeType : MovementModes)
	{
		UPawnPlanatoidMovementMode* newMode = ConstructObject<UPawnPlanatoidMovementMode>(modeType, this);
		if (!currentMode)
		{
			currentMode = newMode;
			currentMode->EnterMode();
		}
	}

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
	return UpdatedPrimitive && GetWorld() && currentMode;
}

FVector UPawnPlanatoidMovementComponent::GetUp() const
{
	return PlanatoidData->GetUpVector();
}

float UPawnPlanatoidMovementComponent::GetMaxGroundSlope() const
{
	return maxGroundSlope;
}

bool UPawnPlanatoidMovementComponent::CanStand(FHitResult groundHit) const
{
	return groundHit.IsValidBlockingHit()
		&& !groundHit.bStartPenetrating
		&& groundHit.GetComponent()->CanCharacterStepUp(GetPawnOwner())
		&& FVector::DotProduct(groundHit.ImpactNormal, PlanatoidData->GetUpVector()) <= maxGroundSlope;
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

	/*Package our tick params for this tick*/
	FTickParams tickParams;
	tickParams.DeltaTime = DeltaTime;
	tickParams.Owner = this;
	tickParams.OwnerPawn = GetPawnOwner();
	tickParams.UpdatedComponent = UpdatedComponent;

	int32 iteration = 0;
	float timeLeft = DeltaTime;
	const float timeSlice = DeltaTime / (float)MaxSimulationSteps;
	float lastTimeRemaining = 0.f;
	while (iteration < MaxSimulationSteps && timeLeft >= 0.f)
	{
		tickParams.Iteration = iteration;
		tickParams.TimeSlice = timeSlice + lastTimeRemaining;
		tickParams.TargetVelocity = CalculateVelocity(tickParams.TimeSlice, Velocity);

		//package the default return values
		FTickReturn returnValue;
		returnValue.OutIteration = iteration;
		returnValue.OutTime = tickParams.TimeSlice;
		returnValue.OutNextMode = NULL;

		//iterate the physics on the current mode
		currentMode->IteratePhysics(tickParams, returnValue);

		//get the amount of time that was left, do not let this be below zero
		lastTimeRemaining = FMath::Max(timeSlice - returnValue.OutTime, 0.f);

		iteration = returnValue.OutIteration;
		timeLeft -= returnValue.OutTime;
		UClass* nextMode = returnValue.OutNextMode;

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

		if (timeLeft < -SMALL_NUMBER)
			UE_LOG(LogTemp, Warning, TEXT("Pawn stepped more than delta time in a single step"));

		iteration++;
	}


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
	FVector newVelocity = inVelocity + accelerationAccumulator * deltaTime;
	accelerationAccumulator = FVector::ZeroVector;

	return newVelocity;
}

void UPawnPlanatoidMovementComponent::MoveComponent(const FVector& delta, FHitResult& outHit)
{
	FRotator rotation = UpdatedComponent->GetComponentRotation();
	if (bAlignToGravity)
	{		
		FMatrix orientationMatrix = PlanatoidData->GetOrientationMatrix();
		rotation = orientationMatrix.Rotator();

		rotation.Yaw += facingDirection;
	}

	SafeMoveUpdatedComponent(delta, rotation, true, outHit);
}
