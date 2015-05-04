// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Engine/GameInstance.h"
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
			if (NavPointPos)
				NavPointPos->SetNavPointRadius(1300.f);
			ASpaceNavPointActor* const NavPointNeg = World->SpawnActor<ASpaceNavPointActor>(NavPointClass, FVector(i * -spacing, j * -spacing, 0.f), FVector::ZeroVector.Rotation(), SpawnParams);
			if (NavPointNeg)
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

void ALostInSpaceGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitPlayer(0);
	InitPlayer(1);

	if (HasOption(OptionsString, TEXT("IsThreePlayer")))
		InitPlayer(2);
}

void ALostInSpaceGameMode::InitPlayer(int playerIndex)
{
	UGameInstance* instance = GetGameInstance();
	if (instance)
	{
		if (playerIndex >= instance->GetNumLocalPlayers())
		{
			FString err;
			instance->CreateLocalPlayer(playerIndex, err, true);
		}
	}
}

UClass* ALostInSpaceGameMode::GetDefaultPawnClassForController(AController* InController)
{
	ALostInSpacePlayerController* pc = Cast<ALostInSpacePlayerController>(InController);
	if (!pc)
		return Super::GetDefaultPawnClassForController(InController);

	return pc->GetDefaultPawnClass();
}

APlayerController* ALostInSpaceGameMode::Login(class UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
	ErrorMessage = GameSession->ApproveLogin(Options);
	if (!ErrorMessage.IsEmpty())
	{
		return NULL;
	}

	ULocalPlayer* localPlayer = Cast<ULocalPlayer>(NewPlayer);
	if (!localPlayer)
		return NULL;

	int32 index = FMath::Min(localPlayer->GetControllerId(), DefaultPlayerControllers.Num() - 1);
	UClass* PCClass = DefaultPlayerControllers[index];
	FActorSpawnParameters params;
	params.Instigator = Instigator;
	APlayerController* NewPlayerController = GetWorld()->SpawnActor<APlayerController>(PCClass, FVector::ZeroVector, FRotator::ZeroRotator, params);

	// Handle spawn failure.
	if (NewPlayerController == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Couldn't spawn player controller of class %s"), PlayerControllerClass ? *PlayerControllerClass->GetName() : TEXT("NULL"));
		ErrorMessage = FString::Printf(TEXT("Failed to spawn player controller"));
		return NULL;
	}

	// Customize incoming player based on URL options
	ErrorMessage = InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	if (!ErrorMessage.IsEmpty())
	{
		return NULL;
	}

	return NewPlayerController;
}

bool ALostInSpaceGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	ALostInSpacePlayerController* pc = Cast<ALostInSpacePlayerController>(Player);
	if (!pc)
		return Super::ShouldSpawnAtStartSpot(Player);

	return pc->CanSpawnAtStartSpot();
}
