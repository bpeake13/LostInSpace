// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "../Gameplay/Projectile.h"
#include "LostInSpaceClasses.h"
#include "Physics/Planatoid/PawnPlanatoidMovementComponent.h"
#include "PlayerPlanetPawn.generated.h"

UCLASS()
class APlayerPlanetPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPlanetPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
	void OnVertical(float val);

	UFUNCTION()
	void OnHorizontal(float val);

	UFUNCTION()
	void OnFire();
private:
	

	float fireCooldownTimer;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UCapsuleComponent* PlayerCapsule;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Rendering")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UPawnPlanatoidMovementComponent* PlanatoidMovement;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	float FireCooldown;
};
