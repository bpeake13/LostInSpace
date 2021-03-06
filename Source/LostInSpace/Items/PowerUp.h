// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "Rocket/PlayerRocket.h"
#include "PowerUp.generated.h"

UCLASS()
class LOSTINSPACE_API APowerUp : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ItemPickup() override;
	virtual void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
