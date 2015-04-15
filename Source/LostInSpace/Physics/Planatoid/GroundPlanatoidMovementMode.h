// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Physics/Planatoid/PawnPlanatoidMovementMode.h"
#include "GroundPlanatoidMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class UGroundPlanatoidMovementMode : public UPawnPlanatoidMovementMode
{
	GENERATED_BODY()

public:
	virtual void IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn) override;

	virtual void EnterMode() override;

	virtual void ExitMode() override;

protected:
	/*Try and move along the current defined ground plane*/
	virtual bool MoveAlongGround(const FTickParams& tickParams, FHitResult& hit);

	/*Tries to move the updated component to the ground, if we find ground then we will return true*/
	virtual bool FindGround(const FTickParams& tickParams, FHitResult& floorResult);

	/*Attempts to step up onto a platform, if it can it sets the new hit result to the new floor*/
	virtual bool TryStepUp(const FTickParams& tickParams, const FHitResult& stepHit, const FVector& delta, FHitResult& hitResult);

protected:
	FHitResult lastGroundHit;//the last valid ground hit that we got
};
