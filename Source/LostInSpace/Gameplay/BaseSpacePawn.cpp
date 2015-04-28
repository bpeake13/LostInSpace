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

	BounceVelocityConsumption = 0.8f;
}

void ABaseSpacePawn::InitRoot()
{
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	checkf(rootPrimitive, TEXT("FATAL ERROR: Root is not of type UPrimitive"));

	rootPrimitive->SetSimulatePhysics(false);

	rootPrimitive->OnComponentHit.AddDynamic(this, &ABaseSpacePawn::OnHit);
}

void ABaseSpacePawn::AddForce(const FVector& force)
{
	MovementComponent->AddForce(force);
}

void ABaseSpacePawn::AddAcceleration(const FVector& acceleration)
{
	MovementComponent->AddAcceleration(acceleration);
}

void ABaseSpacePawn::AddImpulse(const FVector& impulse)
{
	MovementComponent->Velocity += impulse * (1.f / MovementComponent->UpdatedPrimitive->GetMass());
}

void ABaseSpacePawn::AddVelocity(const FVector& velocity)
{
	MovementComponent->Velocity += velocity;
}

void ABaseSpacePawn::OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector reflection = MovementComponent->Velocity.MirrorByVector(NormalImpulse.GetUnsafeNormal());
	FVector bounce = reflection * BounceVelocityConsumption;

	MovementComponent->Velocity = bounce;
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