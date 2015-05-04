// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "LostInSpacePlayerController.h"
#include "Rocket/PlayerPlanetPawn.h"
#include "LostInSpaceCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ALostInSpaceCharacterController : public ALostInSpacePlayerController
{
	GENERATED_BODY()

public:
	virtual UClass* GetDefaultPawnClass() const override;

	virtual bool CanSpawnAtStartSpot() const override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<APlayerPlanetPawn> DefaultPlanetPawn;
};
