// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HierarchicalStateMachine.h"
#include "AI/HSMActionNode.h"
#include "../AIRocket.h"
#include "Seek.generated.h"

/**
 * 
 */


UCLASS()
class LOSTINSPACE_API USeek : public UHSMActionNode
{
	GENERATED_BODY()
protected:
	void Tick(UHierarchicalStateMachine* machine) override;
};
