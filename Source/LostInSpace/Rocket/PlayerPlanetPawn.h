// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "../Gameplay/Projectile.h"
#include "LostInSpaceClasses.h"
#include "PlayerPlanetPawn.generated.h"

UCLASS()
class LOSTINSPACE_API APlayerPlanetPawn : public APawn
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
	void OnDetectionEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnDetectionExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	UFUNCTION()
	void OnVertical(float val);

	UFUNCTION()
	void OnHorizontal(float val);

	UFUNCTION()
	void OnFire();

	float fireCooldownTimer;

	//The center of the camera
	FVector CameraOffset;

	//The height of the camera in the scene
	float BaseCameraHeight;
	float CameraHeight;

	//Calculates the average of the enemies in the detection field
	FVector CalculateCameraOffset();

	//Calculates the average of the enemies in the detection field
	float CalculateCameraHeight();

	//Interpolate camera to the center of the actor field
	void CheckCamera(float DeltaTime);

	//Containers for maximums/minimums of actor locations
	float maxX; float maxY; float minX; float minY;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UCapsuleComponent* PlayerCapsule;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Rendering")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UPawnPlanatoidMovementComponent* PlanatoidMovement;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Collision")
	UBoxComponent* DetectionBox;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Collision")
	TArray<AActor*> DetectedEnemies;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	float FireCooldown;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
