// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "LostInSpaceClasses.h"
#include "Rocket/Navigation/SpaceNavPointActor.h"
#include "LostInSpaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALostInSpaceGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ALostInSpaceGameMode(const FObjectInitializer& objInitializer);

	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<ASpaceNavPointActor> NavPointClass;
};
