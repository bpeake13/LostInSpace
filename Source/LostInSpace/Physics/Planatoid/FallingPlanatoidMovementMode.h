// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Physics/Planatoid/PawnPlanatoidMovementMode.h"
#include "FallingPlanatoidMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class UFallingPlanatoidMovementMode : public UPawnPlanatoidMovementMode
{
	GENERATED_BODY()

public:
	
	virtual void EnterMode() override;

	virtual void ExitMode() override;

	virtual void IteratePhysics(const FTickParams& tickParams, FTickReturn& outReturn) override;
};
