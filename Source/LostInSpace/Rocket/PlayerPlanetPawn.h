// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
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

	void OnDetectionEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnDetectionExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	UFUNCTION()
	void OnVertical(float val);

	UFUNCTION()
	void OnHorizontal(float val);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UCapsuleComponent* PlayerCapsule;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Rendering")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
	UPawnPlanatoidMovementComponent* PlanatoidMovement;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera")
	UBoxComponent* DetectionBox;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
