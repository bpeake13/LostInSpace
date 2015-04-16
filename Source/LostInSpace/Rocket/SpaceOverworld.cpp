// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SpaceOverworld.h"
#include "PlayerRocketController.h"
#include "PlayerRocket.h"
#include "Rocket/Navigation/SpaceNavigationManager.h"

ASpaceOverworld::ASpaceOverworld(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = APlayerRocketController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerRocket"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASpaceOverworld::StartPlay()
{
	Super::StartPlay();

	USpaceNavigationManager::Build();
}

void ASpaceOverworld::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USpaceNavigationManager::Clear();
}


