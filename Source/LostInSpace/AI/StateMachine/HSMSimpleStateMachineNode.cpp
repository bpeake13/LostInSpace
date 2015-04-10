// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SSMNode.h"
#include "HSMSimpleStateMachineNode.h"


void  UHSMSimpleStateMachineNode::Tick(UHierarchicalStateMachine* machine){
	CurrentNode->Tick(this);
}

void UHSMSimpleStateMachineNode::ChangeNode(USSMNode* node){
	CurrentNode = node;
}