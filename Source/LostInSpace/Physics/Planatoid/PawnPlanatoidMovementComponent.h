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

	/*Moves the actor by a set delta*/
	virtual void MoveComponent(const FVector& delta, bool fastMove, FHitResult& outHit);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetUp() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FRotator GetOrientation() const;

	float GetMaxGroundSlope() const;

	float FORCEINLINE GetGroundAccleration() { return GroundAcceleration; }

	float FORCEINLINE GetMaxGroundSpeed() { return MaxGroundSpeed; }

	float FORCEINLINE GetGroundFriction() { return GroundFriction; }

	float FORCEINLINE GetMaxAirMovementSpeed() { return MaxAirMovementSpeed; }

	float FORCEINLINE GetAirControl() { return AirControl; }

	float FORCEINLINE GetMaxFallingSpeed() { return MaxFallSpeed; }

	float FORCEINLINE GetAirFriction() { return AirFriction; }

	float FORCEINLINE GetBrakingDecceleration() { return BrakingDecceleration; }

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

	/*What is the acceleration from movement on the ground*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float GroundAcceleration;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxGroundSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxGroundAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float GroundFriction;

	/*The max speed that we can move while in the air*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxAirMovementSpeed;

	/*While in the air how much control do we have over our player*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AirControl;

	/*The max speed we can fall at, for lateral movement speed see MaxAirSpeed*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxFallSpeed;

	/*The amount of restraint to moving*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AirFriction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BrakingDecceleration;

	/*The max number of steps we can take inside of a single frame*/
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	int32 MaxSimulationSteps;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	TArray<TSubclassOf<UPawnPlanatoidMovementMode>> MovementModes;
private:
	UPROPERTY(Transient)
	UPlanatoidDataComponent* PlanatoidData;

private:
	FVector forceAccumulator;
	FVector accelerationAccumulator;

	bool bForceDirty;

	float facingDirection;

	/*The max slope for the ground*/
	float groundSlope;
private:
	TMap<UClass*, UPawnPlanatoidMovementMode*> modeMap;

	UPawnPlanatoidMovementMode* currentMode;
};
