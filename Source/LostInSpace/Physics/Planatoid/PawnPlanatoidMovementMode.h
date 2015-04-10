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
	/*The target velocity that we calculated*/
	FVector TargetVelocity;
};

USTRUCT()
struct FTickReturn
{
	GENERATED_USTRUCT_BODY()

public:
	float OutTime;
	int32 OutIteration;
	UClass* OutNextMode;
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

protected:
	virtual FVector CalculateDelta(const FTickParams& tickParams, float deltaTime) const;
};
