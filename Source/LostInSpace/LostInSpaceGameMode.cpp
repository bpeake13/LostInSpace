// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Rocket/Navigation/SpaceNavigationManager.h"
#include "LostInSpaceGameMode.h"





void ALostInSpaceGameMode::StartPlay()
{
	USpaceNavigationManager::Build();
}
