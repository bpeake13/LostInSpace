// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "BaseSpacePawn.h"


// Sets default values
ABaseSpacePawn::ABaseSpacePawn()
{
 	
}

void ABaseSpacePawn::InitRoot()
{
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	checkf(rootPrimitive, TEXT("FATAL ERROR: Root is not of type UPrimitive"));

	rootPrimitive->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	rootPrimitive->SetSimulatePhysics(true);
	rootPrimitive->SetEnableGravity(false);
	rootPrimitive->SetAngularDamping(10000.f);
	rootPrimitive->SetLinearDamping(0.1f);
}
