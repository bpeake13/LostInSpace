// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PlanatoidDataComponent.h"


// Sets default values for this component's properties
UPlanatoidDataComponent::UPlanatoidDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	SetUpVector(FVector(0, 0, 1));
	currentAffector = NULL;
}

void UPlanatoidDataComponent::SetUpVector(const FVector& up)
{
	this->up = up;
	
	FVector solverForward = FVector::ForwardVector;
	FVector solverRight = FVector(1, 0, 0);

	if (FVector::DotProduct(this->up, solverForward) < FVector::DotProduct(this->up, solverRight))
	{
		this->right = FVector::CrossProduct(this->up, solverForward);
		this->forward = FVector::CrossProduct(this->right, this->up);
	}
	else
	{
		this->forward = FVector::CrossProduct(solverRight, this->up);
		this->right = FVector::CrossProduct(this->up, this->forward);
	}

	this->orientationMatrix = FRotationMatrix::MakeFromZX(up, forward);
}

FVector UPlanatoidDataComponent::GetUpVector() const
{
	return up;
}

FVector UPlanatoidDataComponent::GetForwardVector() const
{
	return forward;
}

FVector UPlanatoidDataComponent::GetRightVector() const
{
	return right;
}

void UPlanatoidDataComponent::SetCurrentGravitator(UGravitatorComponent* gravitator)
{
	currentAffector = gravitator;
}

UGravitatorComponent* UPlanatoidDataComponent::GetCurrentGravitator() const
{
	return currentAffector;
}

FMatrix UPlanatoidDataComponent::GetOrientationMatrix() const
{
	return orientationMatrix;
}



