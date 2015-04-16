// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SpaceOverworld.generated.h"

/**
 * 
 */
UCLASS()
class ASpaceOverworld : public AGameMode
{
	GENERATED_BODY()
	
public:
	ASpaceOverworld(const FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	
};
