// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HSMNode.h"
#include "HSMConditionNode.generated.h"

/**
 * 
 */
UCLASS()
class UHSMConditionNode : public UHSMNode
{
	GENERATED_BODY()

protected:
	void Execute(UHierarchicalStateMachine* machine) override;

	virtual bool Check(UHierarchicalStateMachine* machine);

	UFUNCTION(BlueprintImplementableEvent, Category = AI, meta = (FriendlyName = "Execute"))
	bool CheckBlueprint(UHierarchicalStateMachine* machine, bool superResult, bool& outResult);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=AI)
	UHSMNode* YesNode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AI)
	UHSMNode* NoNode;
};
