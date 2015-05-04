// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Rocket/Navigation/DestinationPoint.h"
#include "PlayerSpacePawn.h"
#include "LostInSpacePlayerController.h"
#include "LostInSpacePlanetController.generated.h"

/**
 * 
 */
UCLASS()
class ALostInSpacePlanetController : public ALostInSpacePlayerController
{
	GENERATED_BODY()

public:
	void AddScore(float deltaScore);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	float GetScore() const;

	virtual UClass* GetDefaultPawnClass() const override;

	virtual bool CanSpawnAtStartSpot() const override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<APlayerSpacePawn> DefaultPawn;
private:
	float score;
};
