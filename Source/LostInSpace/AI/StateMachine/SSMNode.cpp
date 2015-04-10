// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SSMNode.h"

void USSMNode::Tick(UHSMSimpleStateMachineNode* machine){

}

USSMNode* USSMNode::GetNext(int transitionId)
{
	if (transitionId >= NextStates.Num())
		return NULL;

	return NextStates[transitionId];
}



