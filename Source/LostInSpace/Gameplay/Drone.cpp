// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Rocket/Behaviors/CanSeePlayerPlanet.h"
#include "Rocket/Behaviors/WanderPath.h"
#include "Rocket/Behaviors/AttackPlayerPlanet.h"
#include "Drone.h"

ADrone::ADrone()
	: Super()
{
	VisionRadius = 5000.f;

	UWanderPath* wanderNode = CreateDefaultSubobject<UWanderPath>("Wander");
	wanderNode->MoveAcceleration = 600.f;
	wanderNode->NodeChangeDistance = 300.f;

	UAttackPlayerPlanet* attackNode = CreateDefaultSubobject<UAttackPlayerPlanet>("Attack");

	UCanSeePlayerPlanet* canSeePlanetNode = CreateDefaultSubobject<UCanSeePlayerPlanet>("CanSeePlayer");
	canSeePlanetNode->SetNoNode(wanderNode);
	canSeePlanetNode->SetYesNode(attackNode);

	HSM->SetRoot(canSeePlanetNode);
}


