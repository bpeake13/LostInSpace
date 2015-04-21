// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SpaceOverworld.h"
#include "PlayerRocketController.h"
#include "PlayerRocket.h"
#include "Rocket/Navigation/SpaceNavigationManager.h"
#include "Rocket/Navigation/SpaceNavPoint.h"

ASpaceOverworld::ASpaceOverworld(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if 0
	// use our custom PlayerController class
	PlayerControllerClass = APlayerRocketController::StaticClass();
	NavPointClass = ASpaceNavPointActor::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Pawn'/Game/Alpha/Final/PlayerPlanetPawn_BP.PlayerPlanetPawn_BP.PlayerPlanetPawn_BP_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
#endif

	NavPointClass = ASpaceNavPointActor::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Pawn'/Game/Alpha/Final/PlayerPlanetPawn_BP.PlayerPlanetPawn_BP.PlayerPlanetPawn_BP_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASpaceOverworld::StartPlay()
{
	Super::StartPlay();
	SpawnNavPoints();
	USpaceNavigationManager::Build();
}

void ASpaceOverworld::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USpaceNavigationManager::Clear();
}

void ASpaceOverworld::SpawnNavPoints()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	UWorld* const World = this->GetWorld();
	float spacing = 1000.f;
	int32 width = 30, height = 30;
	for (int32 i = 0; i < width; i++)
	{
		for (int32 j = 0; j < height; j++)
		{
			ASpaceNavPointActor* const NavPointPos = World->SpawnActor<ASpaceNavPointActor>(NavPointClass, FVector(i * spacing, j * spacing, 0.f), FVector::ZeroVector.Rotation(), SpawnParams);
			NavPointPos->SetNavPointRadius(1300.f);
			ASpaceNavPointActor* const NavPointNeg = World->SpawnActor<ASpaceNavPointActor>(NavPointClass, FVector(i * -spacing, j * -spacing, 0.f), FVector::ZeroVector.Rotation(), SpawnParams);
			NavPointNeg->SetNavPointRadius(1300.f);
		}
	}
	
}

