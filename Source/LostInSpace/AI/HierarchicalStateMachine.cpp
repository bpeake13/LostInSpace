// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "HierarchicalStateMachine.h"
#include "HSMActionNode.h"
#include "HSMNode.h"

UHierarchicalStateMachine::UHierarchicalStateMachine()
	: Super()
{
	currentNode = NULL;
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

	if (!bDidEnterNew)
		currentNode->OnLeave(this);
}

AActor* UHierarchicalStateMachine::GetOwnerActor() const
{
	return OwnerActor;
}

void UHierarchicalStateMachine::SetOwnerActor(AActor* actor)
{
	this->OwnerActor = actor;
}

void UHierarchicalStateMachine::ExecuteNode(UHSMNode* node)
{
	UHSMActionNode* leaf = Cast <UHSMActionNode>(node);
	if (leaf)
	{
		bDidEnterNew = true;

		if (leaf != currentNode)
		{
			if (currentNode)
				currentNode->OnLeave(this);
			currentNode = leaf;
			if (currentNode)
				currentNode->OnEnter(this);
		}
	}

	if (node)
		node->Execute(this);
}
