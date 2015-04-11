// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HSMActionNode.h"

UHSMActionNode::UHSMActionNode(){}

void UHSMActionNode::Execute(UHierarchicalStateMachine* machine)
{
	Tick(machine);
	ExecuteBlueprint(machine);
}

void UHSMActionNode::Tick(UHierarchicalStateMachine* machine)
{

}