// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PointGravitatorComponent.h"

void UPointGravitatorComponent::GetUpAtPoint(const FVector& point, FVector& o_upVector)
{
	FVector center = GetComponentLocation();

	o_upVector = (point - center).GetSafeNormal();
}
