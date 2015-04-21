// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HSMConditionNode.h"
#include "CanSeePlayerPlanet.generated.h"

/**
 * 
 */
UCLASS()
class UCanSeePlayerPlanet : public UHSMConditionNode
{
	GENERATED_BODY()

public:
	virtual bool Check(UHierarchicalStateMachine* machine) override;
};
