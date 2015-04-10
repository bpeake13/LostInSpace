// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LostInSpaceClasses.h"
#include "HierarchicalStateMachine.generated.h"

class UHSMNode;

/**
 * A state machine based on a tree structure
 */
UCLASS()
class LOSTINSPACE_API UHierarchicalStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void Tick(float deltaTime);

	float GetDeltaTime() const;
protected:

private:
	//The root node to start execution at
	UHSMNode* Root;

private:
	UHSMNode* currentNode;

	float deltaTime;
};
