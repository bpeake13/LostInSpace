// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/BaseSpacePawn.h"
#include "PlayerSpacePawn.generated.h"

/**
 * 
 */
UCLASS()
class APlayerSpacePawn : public ABaseSpacePawn
{
	GENERATED_BODY()


public:
	APlayerSpacePawn();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

protected:
	UFUNCTION()
	void OnMoveVertical(float val);

	UFUNCTION()
	void OnMoveHorizontal(float val);

	UFUNCTION()
	void OnRotate(float val);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Input")
	float InputAcceleration;
};
