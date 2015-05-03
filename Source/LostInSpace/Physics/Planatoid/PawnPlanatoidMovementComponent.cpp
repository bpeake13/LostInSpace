// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PawnPlanatoidMovementComponent.h"

void FPawnPlanatoidPostPhysicsTickFunction::ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	if (Target)
		Target->LateTickComponent(DeltaTime, TickType, this);
}

FString FPawnPlanatoidPostPhysicsTickFunction::DiagnosticMessage()
{
	return Target->GetFullName() + TEXT("[UPawnPlanatoidMovementComponent::PostPhysicsTick]");
}


UPawnPlanatoidMovementComponent::UPawnPlanatoidMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	PostPhysicsTick.bCanEverTick = true;
	PostPhysicsTick.bStartWithTickEnabled = true;
	PostPhysicsTick.TickGroup = ETickingGroup::TG_PreCloth;

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
		modes.Add(newMode);
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

void UPawnPlanatoidMovementComponent::RegisterComponentTickFunctions(bool bRegister)
{
	Super::RegisterComponentTickFunctions(bRegister);

	if (bRegister)
	{
		if (SetupActorComponentTickFunction(&PostPhysicsTick))
			PostPhysicsTick.Target = this;
	}
	else
	{
		if (PostPhysicsTick.IsTickFunctionRegistered())
			PostPhysicsTick.UnRegisterTickFunction();
	}
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

FVector UPawnPlanatoidMovementComponent::GetForward() const
{
	if (PlanatoidData)
		return PlanatoidData->GetForwardVector();
	return FVector::ForwardVector;
}

FVector UPawnPlanatoidMovementComponent::GetRight() const
{
	if (PlanatoidData)
		return PlanatoidData->GetRightVector();
	return FVector(0, 1, 0);
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
	tickParams.BaseComponent = baseInfo.BaseComponent;
	tickParams.BaseBone = baseInfo.BaseBone;

	FVector orientedInput = FVector::VectorPlaneProject(inputVector, tickParams.Up).GetSafeNormal();
	if (!orientedInput.IsNearlyZero())
	{
		float localForward = FVector::DotProduct(orientedInput, GetForward());
		float localRight = FVector::DotProduct(orientedInput, GetRight());

		facingDirection = -FMath::RadiansToDegrees(FMath::Atan2(localForward, localRight)) + 90.f;
	}

	//forces movement to be applied at the end of the tick
	FScopedMovementUpdate movementScope = FScopedMovementUpdate(UpdatedComponent);

	int32 iteration = 0;
	float timeLeft = DeltaTime;
	float timeSlice = DeltaTime / (float)MaxSimulationSteps;
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
		returnValue.BaseComponent = tickParams.BaseComponent;
		returnValue.BaseBone = tickParams.BaseBone;
		returnValue.OutVelocity = FVector::ZeroVector;

		//iterate the physics on the current mode
		currentMode->IteratePhysics(tickParams, returnValue);

		//get the amount of time that was left, do not let this be below zero
		lastTimeRemaining = FMath::Max(timeSlice - returnValue.OutTime, 0.f);

		iteration = returnValue.OutIteration;
		timeLeft -= returnValue.OutTime;
		UClass* nextMode = returnValue.OutNextMode;

		tickParams.BaseComponent = returnValue.BaseComponent;
		tickParams.BaseBone = returnValue.BaseBone;

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
			SetRelativeVelocity((endPosition - startPosition) * (1.f / returnValue.OutTime));
		else
			SetRelativeVelocity(returnValue.OutVelocity);

		iteration++;
	}

	TryChangeBase(tickParams.BaseComponent, tickParams.BaseBone);

	//remove the acceleration for this frame
	accelerationAccumulator = FVector::ZeroVector;

	if (baseInfo.BaseComponent)
	{
		baseInfo.PrePhysicsWorldPosition = UpdatedComponent->GetComponentLocation();
	}
}

void UPawnPlanatoidMovementComponent::LateTickComponent(float DeltaTime, enum ELevelTick TickType, const FTickFunction *ThisTickFunction)
{
	/*Calculate imparted velocity*/
	if (baseInfo.BaseComponent)
	{
		FVector currentPosition = UpdatedComponent->GetComponentLocation();
		baseInfo.ImpartedVelocity = (currentPosition - baseInfo.PrePhysicsWorldPosition) * (1.f / DeltaTime);
	}
	else
	{
		baseInfo.ImpartedVelocity = FVector::ZeroVector;
	}

	Velocity = baseInfo.RelativeVelocity + baseInfo.ImpartedVelocity;
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

		FVector localForward = FVector::ForwardVector.RotateAngleAxis(facingDirection, FVector::UpVector);
		FVector localRight = FVector(0, 1, 0).RotateAngleAxis(facingDirection, FVector::UpVector);
		FMatrix facingMatrix = FRotationMatrix::MakeFromXY(localForward, localRight);

		FMatrix orientationMatrix = facingMatrix * PlanatoidData->GetOrientationMatrix();
		rotation = orientationMatrix.Rotator();
	}

	MoveUpdatedComponent(delta, rotation, true, &outHit);
	if (outHit.bStartPenetrating)//resolve penetration
	{
		FVector resolveDelta = outHit.ImpactNormal * outHit.PenetrationDepth;
		MoveUpdatedComponent(resolveDelta, rotation, false);
	}
}

void UPawnPlanatoidMovementComponent::TryChangeBase(UPrimitiveComponent* newBase, const FName& newBone)
{
	//if no change has been made, then ignore
	if (baseInfo.BaseComponent == newBase && baseInfo.BaseBone == newBone)
		return;

	if (newBase && MovementBaseUtility::IsDynamicBase(newBase))
		AttatchBase(newBase, newBone);
	else if (newBase)
		DetatchBase();
}

void UPawnPlanatoidMovementComponent::AttatchBase(UPrimitiveComponent* newBase, const FName& newBone)
{
	if (!newBase)
		return;

	//detatch from our current base first
	FVector savedImparted = baseInfo.ImpartedVelocity;
	FVector savedRelative = baseInfo.RelativeVelocity;
	UPrimitiveComponent* savedBase = baseInfo.BaseComponent;
	if (baseInfo.BaseComponent)
		DetatchBase();

	//if we are attatching to the same base then we should restore the old imparted and relative velocity
	if (newBase == savedBase)
	{
		baseInfo.RelativeVelocity = savedRelative;
		baseInfo.ImpartedVelocity = savedImparted;
	}

	baseInfo.BaseComponent = newBase;
	baseInfo.BaseBone = newBone;

	//AddTickPrerequisiteComponent(baseInfo.BaseComponent);
	GetOwner()->AttachRootComponentTo(newBase, newBone, EAttachLocation::KeepWorldPosition, false);
}

void UPawnPlanatoidMovementComponent::DetatchBase()
{
	//if there is no base then ignore the detatch
	if (!baseInfo.BaseComponent)
		return;

	/*Apply imparted velocity to relative velocity*/
	baseInfo.RelativeVelocity = baseInfo.RelativeVelocity + baseInfo.ImpartedVelocity;
	baseInfo.ImpartedVelocity = FVector::ZeroVector;

	//RemoveTickPrerequisiteComponent(baseInfo.BaseComponent);

	/*Remove the base information*/
	baseInfo.BaseComponent = NULL;
	baseInfo.BaseBone = NAME_None;

	//Detatch from the base
	GetOwner()->DetachRootComponentFromParent();
}

FVector UPawnPlanatoidMovementComponent::GetMovementVelocity()
{
	return GetRelativeVelocity();
}

bool UPawnPlanatoidMovementComponent::IsOnGround() const
{
	return Cast<UGroundPlanatoidMovementMode>(currentMode) != NULL;
}