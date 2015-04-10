// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HierarchicalStateMachine.h"
#include "HSMNode.h"



float UHierarchicalStateMachine::GetDeltaTime() const
{
	return this->deltaTime;
}

void UHierarchicalStateMachine::Tick(float deltaTime)
{
	this->deltaTime = deltaTime;

	Root->Execute(this);
}
