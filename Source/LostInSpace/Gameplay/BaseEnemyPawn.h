// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/BaseSpacePawn.h"
#include "AI/HierarchicalStateMachine.h"
#include "Projectile.h"
#include "PlayerSpacePawn.h"
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
	bool MoveTo(const FVector& location, const float acceleration, const float tolerance);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool CanSeePlayer();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void Fire(const FVector& direction);

	FORCEINLINE APlayerSpacePawn* GetTargetEnemy() { return targetPlanet.Get(); }

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

	virtual void OnKilled(float damage, AActor* damageCauser, FDamageEvent const& damageEvent) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	float FireCooldown;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	float Worth;

private:
	float fireCooldownTimer;

	TWeakObjectPtr<APlayerSpacePawn> targetPlanet;
};
