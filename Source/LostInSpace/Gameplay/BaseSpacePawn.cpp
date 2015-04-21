// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "BaseSpacePawn.h"


// Sets default values
ABaseSpacePawn::ABaseSpacePawn()
	: Super()
{
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	bIsDead = false;
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

float ABaseSpacePawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		Kill(Damage, DamageCauser, DamageEvent);
	}

	return Damage;
}

void ABaseSpacePawn::Kill(float damage, AActor* damageCauser, FDamageEvent const& damageEvent)
{
	bIsDead = true;
	OnKilled(damage, damageCauser, damageEvent);
}

void ABaseSpacePawn::OnKilled(float damage, AActor* damageCauser, FDamageEvent const& damageEvent)
{
	Destroy();
}
