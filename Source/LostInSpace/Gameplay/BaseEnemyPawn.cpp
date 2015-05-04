// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "LostInSpacePlanetController.h"
#include "BaseEnemyPawn.h"

ABaseEnemyPawn::ABaseEnemyPawn()
	:Super()
{
	HSM = CreateDefaultSubobject<UHierarchicalStateMachine>("Hierarchical State Machine");

	ProjectileClass = AProjectile::StaticClass();

	VisionRadius = 500.f;

	FireCooldown = 3.f;
	fireCooldownTimer = 3.f;

	MovementComponent->SetMaxSpeed(600.f);

	Worth = 10;
}

void ABaseEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	TActorIterator<APlayerSpacePawn> playerItr(world);
	if (playerItr)
		targetPlanet = *playerItr;

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

bool ABaseEnemyPawn::MoveTo(const FVector& location, const float accelerationVal, const float tolerance)
{
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	checkf(rootPrimitive, TEXT("FATAL ERROR: Root is not of type UPrimitive"));

	FVector offset = location - GetActorLocation();
	float distance = offset.Size();
	if (distance < tolerance)
		return true;

	FVector direction = offset.GetSafeNormal();
	FVector accelerationVector = direction * accelerationVal;

	//calculate time to slow down
	float acceleration = accelerationVal;
	float speed = FVector::DotProduct(rootPrimitive->GetComponentVelocity(), direction);
	if (speed > 0)
	{
		float time = -speed / -acceleration;

		float disntanceToSlow = speed * time + 0.5f * -acceleration * time * time;

		if (distance <= disntanceToSlow + 10.f)
		{
			MovementComponent->AddAcceleration(-accelerationVector);
		}
		else
			MovementComponent->AddAcceleration(accelerationVector);
	}
	else
		MovementComponent->AddAcceleration(accelerationVector);

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
	if (!targetPlanet.IsValid())
		return false;

	return (targetPlanet->GetActorLocation() - GetActorLocation()).SizeSquared() <= this->VisionRadius * this->VisionRadius;
}

void ABaseEnemyPawn::OnKilled(float damage, AActor* damageCauser, FDamageEvent const& damageEvent)
{
	if (!targetPlanet.IsValid())
		return Super::OnKilled(damage, damageCauser, damageEvent);

	ALostInSpacePlanetController* pc = Cast<ALostInSpacePlanetController>(targetPlanet->GetController());
	if (pc)
		pc->AddScore(Worth);

	Super::OnKilled(damage, damageCauser, damageEvent);
}


