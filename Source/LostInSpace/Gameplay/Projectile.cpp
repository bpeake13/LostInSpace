// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SphereComponent->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetAngularDamping(10000.f);
	SphereComponent->SetLinearDamping(0.1f);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = SphereComponent;

	FiringSpeed = 1000.f;
	Damage = 1.f;

	impulseVector = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->AddImpulse(impulseVector * FiringSpeed);

	SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != Instigator && OtherActor != this)
	{
		OtherActor->TakeDamage(Damage, FDamageEvent(), NULL, Instigator);
		Destroy();
	}
}
