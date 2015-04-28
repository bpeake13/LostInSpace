// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "BaseEnemyPawn.h"

ABaseEnemyPawn::ABaseEnemyPawn()
	:Super()
{
	HSM = CreateDefaultSubobject<UHierarchicalStateMachine>("Hierarchical State Machine");

	ProjectileClass = AProjectile::StaticClass();

	VisionRadius = 500.f;

	FireCooldown = 3.f;
	fireCooldownTimer = 3.f;
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

	if (fireCooldownTimer > 0)
		fireCooldownTimer -= DeltaSeconds;
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
			MovementComponent->AddForce(-forceVector);
		}
		else
			MovementComponent->AddForce(forceVector);
	}
	else
		MovementComponent->AddForce(forceVector);

	return false;
}

void ABaseEnemyPawn::Fire(const FVector& direction)
{
	if (fireCooldownTimer > 0)
		return;

	fireCooldownTimer = FireCooldown;

	UWorld* const World = GetWorld();
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FVector FireLocation = rootPrimitive->GetComponentLocation() + direction * (rootPrimitive->Bounds.GetSphere().W + 100.f);

		// spawn the projectile at the muzzle
		AProjectile* const Projectile = World->SpawnActor<AProjectile>(ProjectileClass,	FireLocation, direction.Rotation(), SpawnParams);
	}
}

bool ABaseEnemyPawn::CanSeePlayer()
{
	return GetActorLocation().SizeSquared() <= VisionRadius * VisionRadius;
}


