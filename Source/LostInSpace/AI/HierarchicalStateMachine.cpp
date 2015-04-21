// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HierarchicalStateMachine.h"
#include "HSMNode.h"

UHierarchicalStateMachine::UHierarchicalStateMachine()
	: Super()
{
}

void UHierarchicalStateMachine::SetRoot(UHSMNode* node)
{
	Root = node;
}

float UHierarchicalStateMachine::GetDeltaTime() const
{
	return this->deltaTime;
}

void UHierarchicalStateMachine::Tick(float deltaTime)
{
	this->deltaTime = deltaTime;

	Root->Execute(this);
}

AActor* UHierarchicalStateMachine::GetOwnerActor() const
{
	return OwnerActor;
}

void UHierarchicalStateMachine::SetOwnerActor(AActor* actor)
{
	this->OwnerActor = actor;
}