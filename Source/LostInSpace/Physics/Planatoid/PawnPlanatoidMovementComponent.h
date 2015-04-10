// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PawnPlanatoidMovementMode.h"
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

	virtual void PostLoad() override;

	/*Add a force(M*A) to this object*/
	void AddForce(const FVector& force);

	/*Add an accleration(u/s^2) to this object*/
	void AddAccleration(const FVector& acceleration);

	virtual void MoveComponent(const FVector& delta, FHitResult& outHit);

	FVector GetUp() const;

	float GetMaxGroundSlope() const;

	bool CanStand(FHitResult groundHit) const;
protected:
	UPlanatoidDataComponent* GetPlanatoidData() const;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void ApplyGravity();

	virtual bool ShouldTick() const;

	virtual FVector CalculateVelocity(float deltaTime, const FVector& inVelocity);
private:
	void ApplyForces();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Physics")
	bool bHasGravity;

	UPROPERTY(EditDefaultsOnly, Category = "Physics")
	bool bAlignToGravity;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float InputAcceleration;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxGroundAngle;

	/*The max number of steps we can take inside of a single frame*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	int32 MaxSimulationSteps;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	TArray<TSubclassOf<UPawnPlanatoidMovementMode>> MovementModes;
private:
	UPROPERTY(VisibleDefaultsOnly)
	UPlanatoidDataComponent* PlanatoidData;

private:
	FVector forceAccumulator;
	FVector accelerationAccumulator;

	bool bForceDirty;

	float facingDirection;

	/*The max slope for the ground*/
	float maxGroundSlope;
private:
	TMap<UClass*, UPawnPlanatoidMovementMode*> modeMap;

	UPawnPlanatoidMovementMode* currentMode;
};
