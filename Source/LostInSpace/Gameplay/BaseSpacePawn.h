// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BaseSpacePawn.generated.h"

UCLASS(abstract)
class ABaseSpacePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseSpacePawn();

	void Kill(float damage, AActor* damageCauser, FDamageEvent const& damageEvent);

protected:
	void InitRoot();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	 
	virtual void OnKilled(float damage, AActor* damageCauser, FDamageEvent const& damageEvent);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	float MaxHealth;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	bool bIsDead;
};
