// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LostInSpaceClasses.h"
#include "HierarchicalStateMachine.generated.h"

class UHSMNode;

/**
 * A state machine based on a tree structure
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType)
class LOSTINSPACE_API UHierarchicalStateMachine : public UObject
{
	GENERATED_BODY()

public:
	UHierarchicalStateMachine();

	void SetOwnerActor(AActor* owner);

	AActor* GetOwnerActor() const;

	void SetRoot(UHSMNode* node);

	void Tick(float deltaTime);

	float GetDeltaTime() const;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "State Machine")
	UHSMNode* Root;
private:

	UPROPERTY(Transient)
	AActor* OwnerActor;
private:
	UHSMNode* currentNode;

	float deltaTime;
};
