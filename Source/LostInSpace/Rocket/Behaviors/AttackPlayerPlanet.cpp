// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "AttackPlayerPlanet.h"





void UAttackPlayerPlanet::Tick(UHierarchicalStateMachine* machine)
{
	ABaseEnemyPawn* baseOwner = Cast<ABaseEnemyPawn>(machine->GetOwnerActor());
	UPrimitiveComponent* aiRoot = Cast<UPrimitiveComponent>(machine->GetOwnerActor()->GetRootComponent());

	if (!baseOwner->GetTargetEnemy())
		return;

	float currentDistance = aiRoot->GetComponentLocation().Size();
	const float idealDistance = 1500.f;
	const float stepScale = 2.f;
	const float maxForce = 600.f;
	const float minForce = 100.f;

	UE_LOG(LogTemp, Log, TEXT("%s"), *baseOwner->GetTargetEnemy()->GetActorLocation().ToString());

	FVector direction = (baseOwner->GetTargetEnemy()->GetActorLocation() - aiRoot->GetComponentLocation()).GetUnsafeNormal();

	FVector targetLocation = -direction * idealDistance + baseOwner->GetTargetEnemy()->GetActorLocation();

	ABaseEnemyPawn* ePawn = Cast<ABaseEnemyPawn>(machine->GetOwnerActor());
	ePawn->MoveTo(targetLocation, maxForce, 50.f);
	ePawn->Fire(direction);
}
