// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HSMConditionNode.h"

void UHSMConditionNode::Execute(UHierarchicalStateMachine* machine)
{
	bool result = Check(machine);
	CheckBlueprint(machine, result, result);

	if (result)
		YesNode->Execute(machine);
	else
		NoNode->Execute(machine);
}

bool UHSMConditionNode::Check(UHierarchicalStateMachine* machine)
{
	return false;
}
