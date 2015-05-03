// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PawnPlanatoidMovementMode.h"
#include "PlanatoidDataComponent.h"
#include "PawnPlanatoidMovementComponent.generated.h"

USTRUCT()
struct FPawnPlanatoidPostPhysicsTickFunction : public FTickFunction
{
	GENERATED_USTRUCT_BODY()

		/** CharacterMovementComponent that is the target of this tick **/
	class UPawnPlanatoidMovementComponent* Target;

	/**
	* Abstract function actually execute the tick.
	* @param DeltaTime - frame time to advance, in seconds
	* @param TickType - kind of tick for this frame
	* @param CurrentThread - thread we are executing on, useful to pass along as new tasks are created
	* @param MyCompletionGraphEvent - completion event for this task. Useful for holding the completion of this task until certain child tasks are complete.
	**/
	virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;

	/** Abstract function to describe this tick. Used to print messages about illegal cycles in the dependency graph **/
	virtual FString DiagnosticMessage() override;
};

USTRUCT()
struct FPawnPlanatoidMovementBase
{
	GENERATED_USTRUCT_BODY()

public:
	FPawnPlanatoidMovementBase()
	{
		BaseComponent = NULL;
		BaseBone = NAME_None;
		ImpartedVelocity = FVector::ZeroVector;
		RelativeVelocity = FVector::ZeroVector;
		LastRelativePosition = FVector::ZeroVector;
		PrePhysicsWorldPosition = FVector::ZeroVector;
	}
public:
	/*The base component*/
	UPrimitiveComponent* BaseComponent;
	/*The base bone*/
	FName BaseBone;
	/*The velocity that was imparted from the base*/
	FVector ImpartedVelocity;
	/*The imparted velocity saved from the last calculation, this allows detatchment and re-atatchment inside a single frame*/
	FVector SavedImpartedVelocity;
	/*Velocity caused not by the base*/
	FVector RelativeVelocity;
	/*The last captured relative position*/
	FVector LastRelativePosition;
	/*Our position before the base object ticked*/
	FVector PrePhysicsWorldPosition;
};

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

	virtual void RegisterComponentTickFunctions(bool bRegister) override;

	/*Add a force(M*A) to this object*/
	void AddForce(const FVector& force);

	/*Add an accleration(u/s^2) to this object*/
	void AddAccleration(const FVector& acceleration);

	/*Moves the actor by a set delta*/
	virtual void MoveComponent(const FVector& delta, bool fastMove, FHitResult& outHit);

	/*Sets the base to move relative to*/
	void SetBase(UPrimitiveComponent* newBase);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetUp() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetForward() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetRight() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FRotator GetOrientation() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetMovementVelocity();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsOnGround() const;

	float GetMaxGroundSlope() const;

	float FORCEINLINE GetGroundAccleration() { return GroundAcceleration; }

	float FORCEINLINE GetMaxGroundSpeed() { return MaxGroundSpeed; }

	float FORCEINLINE GetGroundFriction() { return GroundFriction; }

	float FORCEINLINE GetMaxAirMovementSpeed() { return MaxAirMovementSpeed; }

	float FORCEINLINE GetAirControl() { return AirControl; }

	float FORCEINLINE GetMaxFallingSpeed() { return MaxFallSpeed; }

	float FORCEINLINE GetAirFriction() { return AirFriction; }

	float FORCEINLINE GetBrakingDecceleration() { return BrakingDecceleration; }

	FVector FORCEINLINE GetRelativeVelocity() { return baseInfo.RelativeVelocity; }

	void FORCEINLINE SetRelativeVelocity(const FVector& newRelativeVelocity) { baseInfo.RelativeVelocity = newRelativeVelocity; }

	FVector FORCEINLINE GetImpartedVelocity() { return baseInfo.ImpartedVelocity; }

	bool CanStand(FHitResult groundHit) const;

	void LateTickComponent(float DeltaTime, enum ELevelTick TickType, const FTickFunction *ThisTickFunction);
protected:
	UPlanatoidDataComponent* GetPlanatoidData() const;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void ApplyGravity();

	virtual bool ShouldTick() const;

	virtual FVector CalculateVelocity(float deltaTime, const FVector& inVelocity);

	void TryChangeBase(UPrimitiveComponent* newBase, const FName& newBone);

	void AttatchBase(UPrimitiveComponent* newBase, const FName& newBone);

	void DetatchBase();
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

	UPROPERTY()
	FPawnPlanatoidPostPhysicsTickFunction PostPhysicsTick;
private:
	FVector forceAccumulator;
	FVector accelerationAccumulator;

	bool bForceDirty;

	float facingDirection;

	/*The max slope for the ground*/
	float groundSlope;
private:
	TMap<UClass*, UPawnPlanatoidMovementMode*> modeMap;

	UPROPERTY()
	TArray<UPawnPlanatoidMovementMode*> modes;

	UPawnPlanatoidMovementMode* currentMode;

	FPawnPlanatoidMovementBase baseInfo;
};
