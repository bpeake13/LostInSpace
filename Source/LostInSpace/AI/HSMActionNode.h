// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HSMNode.h"
#include "HSMActionNode.generated.h"

/**
 * 
 */
UCLASS()
class UHSMActionNode : public UHSMNode
{
	GENERATED_BODY()
public:
	UHSMActionNode();
protected:

	void Execute(UHierarchicalStateMachine* machine) override;

	virtual void Tick(UHierarchicalStateMachine* machine);

	UFUNCTION(BlueprintImplementableEvent, Category=AI, meta=(FriendlyName = "Execute"))
	void ExecuteBlueprint(UHierarchicalStateMachine* machine);
};
