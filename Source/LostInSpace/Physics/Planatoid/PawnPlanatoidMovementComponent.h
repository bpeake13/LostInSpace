// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PlanatoidDataComponent.h"
#include "PawnPlanatoidMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Planatoid"), meta = (BlueprintSpawnableComponent))
class UPawnPlanatoidMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UPawnPlanatoidMovementComponent();

	virtual void InitializeComponent() override;

	/*Add a force(M*A) to this object*/
	void AddForce(const FVector& force);

	/*Add an accleration(u/s^2) to this object*/
	void AddAccleration(const FVector& acceleration);
protected:
	UPlanatoidDataComponent* GetPlanatoidData() const;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void ApplyGravity();

	virtual bool ShouldTick() const;

	virtual FVector CalculateVelocity(float deltaTime, const FVector& inVelocity);

	virtual FVector CalculateMoveDelta(float deltaTime, const FVector& inVelocity);

	virtual FVector LimitVelocity(FVector& inVelocity) const;

	virtual void MoveComponent(const FVector& delta, const float deltaTime);
private:
	void ApplyForces();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Physics")
	bool bHasGravity;

	UPROPERTY(EditDefaultsOnly, Category = "Physics")
	bool bAlignToGravity;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float InputAcceleration;
private:
	UPROPERTY(VisibleDefaultsOnly)
	UPlanatoidDataComponent* PlanatoidData;

private:
	FVector up;

	FVector forceAccumulator;
	FVector accelerationAccumulator;

	bool bForceDirty;

	float facingDirection;
private:
};
