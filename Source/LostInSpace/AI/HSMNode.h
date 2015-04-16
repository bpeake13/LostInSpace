// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LostInSpaceClasses.h"
#include "HSMNode.generated.h"

class UHierarchicalStateMachine;

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UHSMNode : public UObject
{
	GENERATED_BODY()
	
	
	
public:
	/*Called when this node is executed, can retun a true false value*/
	virtual void Execute(UHierarchicalStateMachine* machine) { check(0); }
};
