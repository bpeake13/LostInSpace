// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Physics/Planatoid/GravitatorComponent.h"
#include "PointGravitatorComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Planatoid"), meta = (BlueprintSpawnableComponent))
class UPointGravitatorComponent : public UGravitatorComponent
{
	GENERATED_BODY()
	
public:
	virtual void GetUpAtPoint(const FVector& point, FVector& o_upVector) override;
};
