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

	MovementComponent = CreateDefaultSubobject<UPlanetBodyMovementComponent>("Movement");
}

void ABaseSpacePawn::InitRoot()
{
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	checkf(rootPrimitive, TEXT("FATAL ERROR: Root is not of type UPrimitive"));

	rootPrimitive->SetSimulatePhysics(false);
}

void ABaseSpacePawn::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(this->RootComponent);
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
	FVector reflection = MovementComponent->Velocity.MirrorByVector(Hit.ImpactNormal);
	FVector bounce = reflection * BounceVelocityConsumption;

	UE_LOG(LogTemp, Log, TEXT("%s"), *NormalImpulse.ToString());

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