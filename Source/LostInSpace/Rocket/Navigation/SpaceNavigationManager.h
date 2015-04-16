// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpaceNavPoint.h"
#include "SpaceNavigationManager.generated.h"

class SearchPathNode
{
public:
	SearchPathNode(float weight, float distance, USpaceNavPoint* node, TSharedPtr<SearchPathNode> previous = NULL)
	{
		this->totalWeight = (previous.IsValid() ? previous->totalWeight : 0) + weight;

		this->score = distance + totalWeight;
		this->node = node;
		this->previous = previous;
	}

	~SearchPathNode()
	{
		previous = NULL;//dereference the shared ptr
	}

	float score;//the score of taking this path to this node
	USpaceNavPoint* node;//the actual node that this search node represents
	TSharedPtr<SearchPathNode> previous;//the previous search node

private:
	float totalWeight;
};

struct SearchPathNodePredicate
{
	bool operator()(TSharedPtr<SearchPathNode> a, TSharedPtr<SearchPathNode> b) const
	{
		return a->score < b->score;
	}
};

/**
 * 
 */
UCLASS(ClassGroup = ("Navigation"), meta = (BlueprintSpawnableComponent))
class USpaceNavigationManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static void AddNavPoint(USpaceNavPoint* newPoint);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static void NavPointsInRange(const FVector& point, float range, TArray<USpaceNavPoint*>& outNavPoints);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static USpaceNavPoint* GetClosestNavPoint(const FVector& point);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static void Build();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static void Clear();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static void ReBuild(UObject* instigator);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static bool GetBestPath(const FVector& start, const FVector& end, TArray<USpaceNavPoint*>& outPath);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static bool GetBestPathFromNodes(const USpaceNavPoint* start, const USpaceNavPoint* end, TArray<USpaceNavPoint*>& outPath);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	static void OptimizePath(const FVector& start, const FVector& end, TArray<USpaceNavPoint*> path);

private:
	static TArray<USpaceNavPoint*> navPoints;
};
