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
	
protected:
	virtual void CalculateUpCheckPoint(const FVector& point, FVector& outEndPoint) override;
};
