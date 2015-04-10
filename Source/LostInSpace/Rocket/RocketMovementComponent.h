// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "RocketMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Movement"), meta = (BlueprintSpawnableComponent))
class URocketMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	URocketMovementComponent();

	virtual void InitializeComponent() override;

	/*Add a force(M*A) to this object*/
	void AddForce(const FVector& force);

	/*Add an accleration(u/s^2) to this object*/
	void AddAccleration(const FVector& acceleration);
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual bool ShouldTick() const;

	virtual FVector CalculateVelocity(float deltaTime, const FVector& inVelocity);

	virtual FVector CalculateMoveDelta(float deltaTime, const FVector& inVelocity);

	virtual FVector LimitVelocity(FVector& inVelocity) const;

	virtual void MoveComponent(const FVector& delta, const float deltaTime);

private:
	void ApplyForces();
private:
	bool bForceDirty;

	FVector forceAccumulator;
	FVector accelerationAccumulator;
};
