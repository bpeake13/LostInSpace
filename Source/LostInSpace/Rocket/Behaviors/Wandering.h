// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HierarchicalStateMachine.h"
#include "AI/HSMActionNode.h"
#include "../AIRocket.h"
#include "Wandering.generated.h"

/**
*
*/

UCLASS()
class LOSTINSPACE_API UWandering : public UHSMActionNode
{
	GENERATED_BODY()
protected:
	int32 cicleRadius;
	int32 circleDistance;
	float randAngleVal;

	FVector wanderAngle;
	FVector circleCenter;

	void Execute(UHierarchicalStateMachine* machine) override;

	void Tick(UHierarchicalStateMachine* machine) override;
};

