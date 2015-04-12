// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "LostInSpaceClasses.h"
#include "PlayerRocket.generated.h"

class APowerUp;

UCLASS()
class LOSTINSPACE_API APlayerRocket : public ADefaultPawn
{
	GENERATED_BODY()
	/** StaticMesh used for the ball */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rocket, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Rocket;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TArray<class APowerUp*> ItemInventory; // Our Inventory

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


public:
	APlayerRocket(const FObjectInitializer& ObjectInitializer);

	/** Returns Rocket subobject **/
	class UStaticMeshComponent* GetRocket() const { return Rocket; }

	/* Returns Rocket Inventory */
	class TArray<class APowerUp*> GetCurrentInventory();

	/** Returns TopDownCameraComponent subobject **/
	class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
