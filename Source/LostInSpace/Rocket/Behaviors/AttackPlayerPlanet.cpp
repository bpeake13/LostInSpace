// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "AttackPlayerPlanet.h"





void UAttackPlayerPlanet::Tick(UHierarchicalStateMachine* machine)
{
	UPrimitiveComponent* aiRoot = Cast<UPrimitiveComponent>(machine->GetOwnerActor()->GetRootComponent());

	float currentDistance = aiRoot->GetComponentLocation().Size();
	const float idealDistance = 1500.f;
	const float stepScale = 2.f;
	const float maxForce = 2000000.f;
	const float minForce = 1000.f;

	FVector direction = -aiRoot->GetComponentLocation().GetUnsafeNormal();

	FVector targetLocation = -direction * idealDistance;

	ABaseEnemyPawn* ePawn = Cast<ABaseEnemyPawn>(machine->GetOwnerActor());
	ePawn->MoveTo(targetLocation, maxForce, 50.f);
	ePawn->Fire(direction);
}
