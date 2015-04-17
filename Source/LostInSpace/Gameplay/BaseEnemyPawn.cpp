// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "BaseEnemyPawn.h"

ABaseEnemyPawn::ABaseEnemyPawn()
{
	HSM = CreateDefaultSubobject<UHierarchicalStateMachine>("Hierarchical State Machine");

	ProjectileClass = AProjectile::StaticClass();
}

void ABaseEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	if (HSM)
		HSM->SetOwnerActor(this);
}

void ABaseEnemyPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HSM)
		HSM->Tick(DeltaSeconds);
}

bool ABaseEnemyPawn::MoveTo(const FVector& location, const float force, const float tolerance)
{
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	checkf(rootPrimitive, TEXT("FATAL ERROR: Root is not of type UPrimitive"));

	FVector offset = location - GetActorLocation();
	float distance = offset.Size();
	if (distance < tolerance)
		return true;

	FVector direction = offset.GetSafeNormal();
	FVector forceVector = direction * force;

	//calculate time to slow down
	float acceleration = force / rootPrimitive->GetMass();
	float speed = FVector::DotProduct(rootPrimitive->GetComponentVelocity(), direction);
	if (speed > 0)
	{
		float time = -speed / -acceleration;

		float disntanceToSlow = speed * time + 0.5f * -acceleration * time * time;

		if (distance <= disntanceToSlow + 10.f)
		{
			rootPrimitive->AddForce(-forceVector);
		}
		else
			rootPrimitive->AddForce(forceVector);
	}
	else
		rootPrimitive->AddForce(forceVector);

	return false;
}

void ABaseEnemyPawn::Fire()
{
	UWorld* const World = GetWorld();
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector FireLocation = rootPrimitive->GetForwardVector();
		FRotator FireRotation = rootPrimitive->GetComponentRotation();

		// spawn the projectile at the muzzle
		AProjectile* const Projectile = World->SpawnActor<AProjectile>(ProjectileClass, FireLocation, FireRotation, SpawnParams);
		Projectile->SetImpulseVector(FireRotation.Vector());
	}
}


