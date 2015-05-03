// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Rocket/Navigation/DestinationPoint.h"
#include "LostInSpaceCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ALostInSpaceCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	void AddScore(float deltaScore);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	float GetScore() const;

	virtual void PlayerTick(float DeltaTime) override;

private:
	void ChangeToNextDestination();
private:
	float score;

	TWeakObjectPtr<UDestinationPoint> destination;
};
