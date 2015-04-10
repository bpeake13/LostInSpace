// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SpaceNavPoint.h"
#include "SpaceNavigationManager.h"

TArray<USpaceNavPoint*> USpaceNavigationManager::navPoints = TArray<USpaceNavPoint*>();

void USpaceNavigationManager::Build()
{
	for (USpaceNavPoint* navPoint : navPoints)
	{
		navPoint->Build();
	}
}

void USpaceNavigationManager::ReBuild(UObject* instigator)
{
	navPoints.Empty();

	for (TActorIterator<AActor> ActorItr(instigator->GetWorld()); ActorItr; ++ActorItr)
	{
		TArray<UActorComponent*> components = ActorItr->GetComponentsByClass(USpaceNavPoint::StaticClass());
		for (UActorComponent* component : components)
		{
			USpaceNavPoint* navPoint = Cast<USpaceNavPoint>(component);
			if (!navPoint)
				continue;

			navPoints.Add(navPoint);
		}
	}

	Build();
}

void USpaceNavigationManager::Clear()
{
	navPoints.Empty();
}

void USpaceNavigationManager::NavPointsInRange(const FVector& point, float range, TArray<USpaceNavPoint*>& outNavPoints)
{
	outNavPoints.Empty();

	float rangeSq = range * range;

	for (USpaceNavPoint* navPoint : navPoints)
	{
		FVector delta = navPoint->GetComponentLocation() - point;
		if (delta.SizeSquared() <= rangeSq)
			outNavPoints.Add(navPoint);
	}
}

USpaceNavPoint* USpaceNavigationManager::GetClosestNavPoint(const FVector& point)
{
	float shortestDistanceSq = MAX_FLT;
	USpaceNavPoint* closestPoint = NULL;
	for (USpaceNavPoint* navPoint : navPoints)
	{
		float distSq = (navPoint->GetComponentLocation() - point).SizeSquared();

		if (distSq < shortestDistanceSq)
		{
			closestPoint = navPoint;
			shortestDistanceSq = distSq;
		}
	}

	return closestPoint;
}

void USpaceNavigationManager::AddNavPoint(USpaceNavPoint* newPoint)
{
	if (navPoints.Contains(newPoint))
		return;

	navPoints.Add(newPoint);
}

bool USpaceNavigationManager::GetBestPath(const FVector& start, const FVector& end, TArray<FVector>& outPath)
{
	outPath.Empty();

	USpaceNavPoint* startPoint = GetClosestNavPoint(start);
	USpaceNavPoint* endPoint = GetClosestNavPoint(end);

	if (!startPoint || !endPoint)
		return false;

	FVector endPointLocation = endPoint->GetComponentLocation();

	outPath.Add(start);

	TArray<USpaceNavPoint*> closed;//all visited nav points
	TArray<SearchPathNode*> open;//all edges to go to next

	SearchPathNode* current = new SearchPathNode(0, 0, startPoint);//create the root search path node
	do 
	{
		if (current)
		{
			//add all edges that go to open nodes to the open list
			for (FSpaceNavEdge edge : current->node->Edges)
			{
				if (closed.Contains(edge.Target))
					continue;

				float distance = (edge.Target->GetComponentLocation() - endPointLocation).Size();
				SearchPathNode* openPath = new SearchPathNode(edge.Weight, distance, edge.Target, current);//create a path from the current one to add to the open list

				SearchPathNode* foundNode = NULL;
				for (SearchPathNode* node : open)
				{
					if (node->node == edge.Target)
					{
						foundNode = node;
						break;
					}
				}

				//if the node is already in the open list then we will see if our path is better
				if (foundNode)
				{
					if (foundNode->score > openPath->score)//check if we have a better score
					{
						//when we do have the better score then remove the old from the open list and add ours
						open.Remove(foundNode);
						open.HeapPush(foundNode, SearchPathNodePredicate());
					}
				}
				else
				{
					open.HeapPush(openPath, SearchPathNodePredicate());
				}
			}
		}

		//add the current node to the closed list and remove it from the opened
		closed.Add(current->node);
		open.Remove(current);

		if (closed.Contains(endPoint))//if the end point was added to the closed list then break
			break;

		if (open.Num() != 0)
			current = open.HeapTop();

	} while (open.Num() != 0);

	if (!current)
		return false;

	for (SearchPathNode* searchNode = current; searchNode; searchNode = searchNode->previous)
	{
		outPath.Insert(searchNode->node->GetComponentLocation(), 1);
	}

	outPath.Add(end);
	return true;
}