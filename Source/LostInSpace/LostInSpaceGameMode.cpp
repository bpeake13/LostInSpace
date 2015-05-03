// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Rocket/Navigation/SpaceNavigationManager.h"
#include "Rocket/Navigation/SpaceNavPoint.h"
#include "Rocket/Navigation/SpaceNavPointActor.h"
#include "LostInSpaceGameMode.h"




ALostInSpaceGameMode::ALostInSpaceGameMode(const FObjectInitializer& objInitializer)
	:Super(objInitializer)
{
	NavPointClass = ASpaceNavPointActor::StaticClass();
}

void ALostInSpaceGameMode::StartPlay()
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

	USpaceNavigationManager::Build();
	
	Super::StartPlay();
}

void ALostInSpaceGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USpaceNavigationManager::Clear();
}