// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "LostInSpacePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALostInSpacePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual UClass* GetDefaultPawnClass() const;

	virtual bool CanSpawnAtStartSpot() const;
};
