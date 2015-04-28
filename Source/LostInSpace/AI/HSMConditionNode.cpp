// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HSMConditionNode.h"


UHSMConditionNode::UHSMConditionNode()
{
	YesNode = NULL;
	NoNode = NULL;
}

void UHSMConditionNode::Execute(UHierarchicalStateMachine* machine)
{
	bool result = Check(machine);
	CheckBlueprint(machine, result, result);

	if (result)
		machine->ExecuteNode(YesNode);
	else
		machine->ExecuteNode(NoNode);
}

bool UHSMConditionNode::Check(UHierarchicalStateMachine* machine)
{
	return false;
}

void UHSMConditionNode::SetYesNode(UHSMNode* newYesNode)
{
	this->YesNode = newYesNode;
}

void UHSMConditionNode::SetNoNode(UHSMNode* newNoNode)
{
	this->NoNode = newNoNode;
}