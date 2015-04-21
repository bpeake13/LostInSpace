// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HSMActionNode.h"
#include "AttackPlayerPlanet.generated.h"

/**
 * 
 */
UCLASS()
class UAttackPlayerPlanet : public UHSMActionNode
{
	GENERATED_BODY()
public:
	virtual void Tick(UHierarchicalStateMachine* machine) override;
	
};
