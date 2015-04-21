// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/HSMActionNode.h"
#include "Rocket/Navigation/SpaceNavPoint.h"
#include "WanderPath.generated.h"

/**
 * 
 */
UCLASS()
class UWanderPath : public UHSMActionNode
{
	GENERATED_BODY()
	
public:
	UWanderPath();

	virtual void Tick(UHierarchicalStateMachine* machine) override;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Navigation")
	float ReCalculateTime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Navigation")
	float FixTime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Navigation")
	float MoveForce;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Navigation")
	float MaxWanderRange;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	float NodeChangeDistance;
private:
	//timer for when to recalculate the path
	UPROPERTY(Transient)
	float reCalcTimer;

	//timer for when to fix the path
	UPROPERTY(Transient)
	float fixTimer;

	//The current path to follow
	TArray<USpaceNavPoint*> path;
};
