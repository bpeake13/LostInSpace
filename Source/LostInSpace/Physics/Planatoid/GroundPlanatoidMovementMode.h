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
	/*Tries to move the updated component to the ground, if we find ground then we will return true*/
	virtual bool FindGround(const FTickParams tickParams, FHitResult& floorResult);

protected:
	FHitResult lastGroundHit;//the last valid ground hit that we got
};
