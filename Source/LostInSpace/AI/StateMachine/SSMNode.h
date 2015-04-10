// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LostInSpaceClasses.h"
#include "SSMNode.generated.h"

class UHSMSimpleStateMachineNode;

/**
 * 
 */
UCLASS()
class LOSTINSPACE_API USSMNode : public UObject
{
	GENERATED_BODY()

public:
	void Tick(UHSMSimpleStateMachineNode* machine);

protected:
	UFUNCTION(BlueprintCallable, Category=AI)
	USSMNode* GetNext(int32 transitionId);
private:
	UPROPERTY(EditAnywhere)
	TArray<USSMNode*> NextStates;
};
