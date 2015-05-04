// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "LostInSpaceClasses.h"
#include "Rocket/Navigation/SpaceNavPointActor.h"
#include "Gameplay/PlayerSpacePawn.h"
#include "Rocket/PlayerPlanetPawn.h"
#include "Gameplay/LostInSpacePlayerController.h"
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

	virtual void BeginPlay() override;

	void InitPlayer(int32 index);

	virtual UClass* GetDefaultPawnClassForController(AController* InController) override;

	virtual APlayerController* Login(class UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage) override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<ALostInSpacePlayerController> NavPointClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TArray<TSubclassOf<ALostInSpacePlayerController>> DefaultPlayerControllers;
};
