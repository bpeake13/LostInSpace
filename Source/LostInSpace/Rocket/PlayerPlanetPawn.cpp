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

}

// Called to bind functionality to input
void APlayerPlanetPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Vertical", this, &APlayerPlanetPawn::OnVertical);
	InputComponent->BindAxis("Horizontal", this, &APlayerPlanetPawn::OnHorizontal);

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

	UE_LOG(LogTemp, Warning, TEXT("%s"), *moveDirection.ToString());

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

