// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpaceNavPoint.h"
#include "SpaceNavPointActor.generated.h"

UCLASS()
class ASpaceNavPointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceNavPointActor();

	float GetNavPointRadius() const;
	void SetNavPointRadius(float val);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Navigation")
	USpaceNavPoint* NavComponent;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	UBillboardComponent* Sprite;

#endif
};
