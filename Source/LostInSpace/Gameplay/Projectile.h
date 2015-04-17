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

	FORCEINLINE void SetImpulseVector(FVector impulseVector){ this->impulseVector = impulseVector; }
	FORCEINLINE FVector GetImpulseVector(){ return impulseVector; }

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	float FiringSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	float Damage;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* SphereComponent;

	UFUNCTION()
	virtual void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	
	FVector impulseVector;
};
