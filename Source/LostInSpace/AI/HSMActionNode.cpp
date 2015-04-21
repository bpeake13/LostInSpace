// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HSMActionNode.h"

UHSMActionNode::UHSMActionNode()
: Super()
{}

void UHSMActionNode::Execute(UHierarchicalStateMachine* machine)
{
	Tick(machine);
	ExecuteBlueprint(machine);
}

void UHSMActionNode::Tick(UHierarchicalStateMachine* machine)
{

}