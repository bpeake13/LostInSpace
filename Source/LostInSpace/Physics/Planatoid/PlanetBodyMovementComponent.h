// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/MovementComponent.h"
#include "PlanetBodyMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Planatoid"), meta = (BlueprintSpawnableComponent))
class UPlanetBodyMovementComponent : public UMovementComponent
{
	GENERATED_UCLASS_BODY()	


public:
	virtual void AddForce(const FVector& forceVector);
	virtual void AddAcceleration(const FVector& accelerationVector);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	FORCEINLINE void SetMaxSpeed(float maxSpeed) { MaxSpeed = maxSpeed; }

protected:
	virtual float SimulateTick(float deltaTime);

private:
	void ApplyForces();

	/*Calculate our new velocity*/
	FVector CalculateVelocity(float deltaTime);

	FVector CalculateDelta(const FVector& startVelocity, const FVector& endVelocity, float deltaTime);

	void LimitVelocity(FVector& velocityVector);

	FVector ApplyBreaking(const FVector& velocity, float deltaTimes);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Physics)
	int32 MaxSimulationSteps;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Physics)
	float MaxSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Physics)
	float MaxRotationalSpeed;

	/*The frictional force caused just by moving*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Physics)
	float LinearFriction;

	/*The force to apply when no outside accleration is being applied, or when we are being messaged to break*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Physics)
	float BreakingForce;
private:
	FVector forceAccumulator;
	FVector accelerationAccumulator;
	bool bForceDirty;

	FVector torqueAccumulator;
	FVector rotationalAccelerationAccumulator;
	bool bTorqueDirty;

	bool bBreaking;
};
