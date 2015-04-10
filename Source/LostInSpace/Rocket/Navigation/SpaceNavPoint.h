// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "SpaceNavPoint.generated.h"

class USpaceNavPoint;

USTRUCT()
struct FSpaceNavEdge
{
	GENERATED_USTRUCT_BODY()

	FSpaceNavEdge()
	{
		Weight = 0;
		Target = NULL;
	}

	explicit FSpaceNavEdge(float weight, USpaceNavPoint* target)
	{
		Weight = weight;
		Target = target;
	}

	UPROPERTY()
	float Weight;

	UPROPERTY()
	USpaceNavPoint* Target;
};


UCLASS( ClassGroup=(Navigation), meta=(BlueprintSpawnableComponent) )
class USpaceNavPoint : public USceneComponent
{
	friend class USpaceNavigationManager;
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpaceNavPoint();

	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void OnComponentCreated() override;

	virtual void Build();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pathfinding")
	float Radius;
private:
	UPROPERTY()
	bool bDirty;

	UPROPERTY()
	TArray<FSpaceNavEdge> Edges;
};
