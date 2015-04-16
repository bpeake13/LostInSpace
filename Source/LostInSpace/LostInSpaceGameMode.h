// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "LostInSpaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALostInSpaceGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void StartPlay() override;
	
};
