// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Rocket/Navigation/SpaceNavPointActor.h"
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Start Up")
	TSubclassOf<ASpaceNavPointActor> NavPointClass;

private:
	void SpawnNavPoints();
	
};
