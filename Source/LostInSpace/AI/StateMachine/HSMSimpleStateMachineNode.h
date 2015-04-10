// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HSMNode.h"
#include "AI/HSMActionNode.h"
#include "AI/HierarchicalStateMachine.h"
#include "HSMSimpleStateMachineNode.generated.h"

class USSMNode;

/**
 * 
 */
UCLASS()
class LOSTINSPACE_API UHSMSimpleStateMachineNode : public UHSMActionNode
{
	GENERATED_BODY()
public:
	void ChangeNode(USSMNode* node);
	void Tick(UHierarchicalStateMachine* machine) override;
protected:
	UPROPERTY()
		USSMNode* CurrentNode;
};
