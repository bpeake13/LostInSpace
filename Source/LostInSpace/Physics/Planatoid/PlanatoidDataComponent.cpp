// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "GravitatorComponent.h"
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
	this->up = up.GetUnsafeNormal();
	
	FVector solverForward = FVector::ForwardVector;
	FVector solverRight = FVector(0, 1, 0);

	if (FMath::Abs(FVector::DotProduct(this->up, solverForward)) < FMath::Abs(FVector::DotProduct(this->up, solverRight)))
	{
		this->right = FVector::CrossProduct(this->up, solverForward).GetUnsafeNormal();
		this->forward = FVector::CrossProduct(this->right, this->up).GetUnsafeNormal();
	}
	else
	{
		this->forward = FVector::CrossProduct(solverRight, this->up).GetUnsafeNormal();
		this->right = FVector::CrossProduct(this->up, this->forward).GetUnsafeNormal();
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
	if (currentAffector)
	{
		currentAffector->RemoveAffector(this);
		GetOwner()->RemoveTickPrerequisiteComponent(currentAffector);
	}

	currentAffector = gravitator;

	currentAffector->AddAffector(this);
	
	GetOwner()->AddTickPrerequisiteComponent(gravitator);
}

UGravitatorComponent* UPlanatoidDataComponent::GetCurrentGravitator() const
{
	return currentAffector;
}

FMatrix UPlanatoidDataComponent::GetOrientationMatrix() const
{
	return orientationMatrix;
}

void UPlanatoidDataComponent::SetAtPoint(FVector& newPoint)
{
	if (currentAffector)
	{
		FVector newUp;
		currentAffector->GetUpAtPoint(newPoint, newUp);
		SetUpVector(newUp);
	}
}



