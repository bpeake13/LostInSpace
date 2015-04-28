// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Physics/Planatoid/PawnPlanatoidMovementComponent.h"
#include "PlayerPlanetPawn.h"

// Sets default values
APlayerPlanetPawn::APlayerPlanetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	PlayerCapsule->SetCapsuleHalfHeight(40.f);
	PlayerCapsule->SetCapsuleRadius(10.f);
	PlayerCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = PlayerCapsule;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Mesh->SetRelativeLocation(FVector(0, 0, 0));
	Mesh->AttachParent = RootComponent;

	PlanatoidMovement = CreateDefaultSubobject<UPawnPlanatoidMovementComponent>(TEXT("PlanetMovement"));

	ProjectileClass = AProjectile::StaticClass();
	FireCooldown = 3.f;
	fireCooldownTimer = 3.f;
}

// Called when the game starts or when spawned
void APlayerPlanetPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPlanetPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (fireCooldownTimer > 0)
		fireCooldownTimer -= DeltaTime;
}

// Called to bind functionality to input
void APlayerPlanetPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	//InputComponent->BindAxis("Vertical", this, &APlayerPlanetPawn::OnVertical);
	//InputComponent->BindAxis("Horizontal", this, &APlayerPlanetPawn::OnHorizontal);
	//InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPlanetPawn::OnFire);
	Super::SetupPlayerInputComponent(InputComponent);
}

void APlayerPlanetPawn::OnVertical(float val)
{
	FVector up = PlanatoidMovement->GetUp();
	
	//how much of our up is going towards the planets up
	float planetUpDot = FVector::DotProduct(up, FVector(1, 0, 0));

	float rotationDegrees = FMath::RadiansToDegrees(FMath::Asin(planetUpDot));
	FRotator directionRotator = FRotator(-rotationDegrees, 0, 0);
	FVector moveDirection = directionRotator.RotateVector(FVector::ForwardVector);

	PlanatoidMovement->AddInputVector(moveDirection * val);
}

void APlayerPlanetPawn::OnHorizontal(float val)
{
	FVector up = PlanatoidMovement->GetUp();

	//how much of our up is going towards the planets up
	float planetUpDot = FVector::DotProduct(up, FVector(0, 1, 0));

	float rotationDegrees = FMath::RadiansToDegrees(FMath::Asin(planetUpDot));
	FRotator directionRotator = FRotator();
	FVector moveDirection = directionRotator.RotateVector(FVector(0, 1, 0));

	PlanatoidMovement->AddInputVector(moveDirection * val);
}

void APlayerPlanetPawn::OnFire()
{
	FVector actorForwardVector = this->GetActorForwardVector();
	FVector direction = FVector(actorForwardVector.X, actorForwardVector.Y, 0.f);

	
	if (fireCooldownTimer > 0)
		return;
	
	fireCooldownTimer = FireCooldown;

	UWorld* const World = GetWorld();
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "FKEY PRESSED");

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FVector FireLocation = rootPrimitive->GetComponentLocation() + direction * (rootPrimitive->Bounds.GetSphere().W + 100.f);

		// spawn the projectile at the muzzle
		AProjectile* const Projectile = World->SpawnActor<AProjectile>(ProjectileClass, FireLocation, direction.Rotation(), SpawnParams);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Projectile Fired!");

	}
}
