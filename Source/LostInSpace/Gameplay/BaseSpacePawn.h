// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "LostInSpaceClasses.h"
#include "Physics/Planatoid/PlanetBodyMovementComponent.h"
#include "BaseSpacePawn.generated.h"

UCLASS(abstract)
class ABaseSpacePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseSpacePawn();

	void Kill(float damage, AActor* damageCauser, FDamageEvent const& damageEvent);

	void AddForce(const FVector& force);

	void AddAcceleration(const FVector& acceleration);

	void AddImpulse(const FVector& impulse);

	void AddVelocity(const FVector& velocity);

protected:
	void InitRoot();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	 
	virtual void OnKilled(float damage, AActor* damageCauser, FDamageEvent const& damageEvent);

	UFUNCTION()
	void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float BounceVelocityConsumption;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	bool bIsDead;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UPlanetBodyMovementComponent* MovementComponent;
};
