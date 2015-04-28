// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
	: Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = SphereComponent;
	SphereComponent->bGenerateOverlapEvents = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->InitialSpeed = 5000.f;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->bShouldBounce = false;

	Damage = 1.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	MovementComponent->Velocity = this->GetActorRotation().Vector() * MovementComponent->InitialSpeed;
}

void AProjectile::OnBeginOverlap(AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && OtherActor != Instigator && OtherActor != this)
	{
		FPointDamageEvent damageEvent = FPointDamageEvent();
		damageEvent.Damage = Damage;
		damageEvent.HitInfo = SweepResult;
		OtherActor->TakeDamage(Damage, damageEvent, NULL, Instigator);
		Destroy();
	}
}


