// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	float FiringSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	float Damage;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* SphereComponent;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* MovementComponent;

	UFUNCTION()
	virtual void OnBeginOverlap(AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
};
