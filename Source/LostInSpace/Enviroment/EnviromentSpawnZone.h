// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "EnviromentSpawnZone.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEnviromentSpawnZone : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnviromentSpawnZone();

	// Called when the game starts
	virtual void InitializeComponent() override;

protected:
	void SpawnInstances();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> SpawnType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	float SpawnRadius;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	int32 SpawnCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawning")
	float ScanDistance;
};
