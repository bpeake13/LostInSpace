// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/BaseSpacePawn.h"
#include "AI/HierarchicalStateMachine.h"
#include "Projectile.h"
#include "BaseEnemyPawn.generated.h"

/**
 * 
 */
UCLASS()
class ABaseEnemyPawn : public ABaseSpacePawn
{
	GENERATED_BODY()
public:
	ABaseEnemyPawn();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool MoveTo(const FVector& location, const float force, const float tolerance);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool CanSeePlayer();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void Fire(const FVector& direction);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AI")
	float VisionRadius;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "AI")
	UHierarchicalStateMachine* HSM;

	//Movement Speed for Rocket
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	//Tick function
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	float FireCooldown;

private:
	float fireCooldownTimer;
};
