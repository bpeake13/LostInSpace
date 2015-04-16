// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Physics/Planatoid/PawnPlanatoidMovementComponent.h"
#include "PlayerPlanetPawn.h"


// Sets default values
APlayerPlanetPawn::APlayerPlanetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	FRotator rotation = PlanatoidMovement->GetOrientation();
	rotation.Pitch = 0;
	rotation.Roll = 0;

	PlanatoidMovement->AddInputVector(FVector(0, val, 0));
}

void APlayerPlanetPawn::OnHorizontal(float val)
{

}

