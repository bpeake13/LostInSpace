// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "../Rocket/PlayerPlanetPawn.h"
#include "Collectible.generated.h"

UCLASS()
class LOSTINSPACE_API ACollectible : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollectible();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Model")
	UStaticMeshComponent* Mesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ItemPickup() override;
	virtual void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};

