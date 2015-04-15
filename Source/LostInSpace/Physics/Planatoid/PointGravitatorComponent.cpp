// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PointGravitatorComponent.h"

void UPointGravitatorComponent::CalculateUpCheckPoint(const FVector& point, FVector& outEndPoint)
{
	outEndPoint = GetComponentLocation();
}
