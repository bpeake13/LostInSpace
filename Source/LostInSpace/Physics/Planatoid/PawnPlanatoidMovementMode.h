// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LostInSpaceClasses.h"
#include "PawnPlanatoidMovementMode.generated.h"

class UPawnPlanatoidMovementComponent;

USTRUCT()
struct FTickParams
{
	GENERATED_USTRUCT_BODY()

public:
	/*The slice of time that this iteration is*/
	float TimeSlice;
	/*The total time that this tick is*/
	float DeltaTime;
	/*The iteration count of this tick*/
	int32 Iteration;
	/*The movement component that owns this movment mode*/
	UPawnPlanatoidMovementComponent* Owner;
	/*The component that is being updated*/
	USceneComponent* UpdatedComponent;
	/*The pawn that owns this movement mode*/
	APawn* OwnerPawn;
	/*The actors current up vector*/
	FVector Up;
	/*The acceleration*/
	FVector Acceleration;
	/*The last input vector consumed*/
	FVector InputVector;
};

USTRUCT()
struct FTickReturn
{
	GENERATED_USTRUCT_BODY()

public:
	/*The amount of time this tick consumed*/
	float OutTime;
	/*The updated iteration count*/
	int32 OutIteration;
	/*The next mode to go into*/
	UClass* OutNextMode;
	/*Was there a hit during this update*/
	bool bWasHit;
	/*The desired velocity - if bWasHit then this velocity will not be used*/
	FVector OutVelocity;
};

/**
 * 
 */
UCLASS(abstract)
class UPawnPlanatoidMovementMode : public UObject
{
	GENERATED_BODY()

public:
	virtual void EnterMode();

	virtual void ExitMode();

	virtual void IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn);

	virtual FVector CalculateVelocity(const FVector& inVelocity, const FVector& inAcceleration, const float deltaTime) const;
protected:
	virtual FVector CalculateDelta(const FVector& startVelocity, FVector& endVelocity, float deltaTime) const;
};
