// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "PlayerRocketController.generated.h"

UCLASS()
class APlayerRocketController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerRocketController(const FObjectInitializer& ObjectInitializer);

	/** Force to apply when trying to move Rocket */
	UPROPERTY(EditAnywhere, Category = Rocket)
	float PushForce;

	/** Force to apply when trying to turn Rocket */
	UPROPERTY(EditAnywhere, Category = Rocket)
	float TorqueForce;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	//Variables to control rotation and rotation speed
	float rotationVal;
	float rotationSpeed;

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	void MoveForward(float val);
	void MoveRight(float val);

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


