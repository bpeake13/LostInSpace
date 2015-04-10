// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LostInSpace.h"
#include "PlayerRocketController.h"
#include "PlayerRocket.h"
#include "AI/Navigation/NavigationSystem.h"

APlayerRocketController::APlayerRocketController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	PrimaryActorTick.bCanEverTick = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	TorqueForce = 5000000.0f;
	PushForce = 500000.0f;

	rotationVal = 0.f;
	rotationSpeed = 5.0f;
}

void APlayerRocketController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

}

void APlayerRocketController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APlayerRocketController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APlayerRocketController::OnSetDestinationReleased);

	InputComponent->BindAxis("MoveRight", this, &APlayerRocketController::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &APlayerRocketController::MoveForward);
}

void APlayerRocketController::MoveForward(float val){
	//const FVector Force = FVector(1.f * val * PushForce, 0.f, 0.f);
	APlayerRocket* const Pawn = (APlayerRocket*)GetPawn();
	const FVector Force = Pawn->GetActorForwardVector() * val * PushForce;
	Pawn->GetRocket()->AddForce(Force);
}

void APlayerRocketController::MoveRight(float val){
	APlayerRocket* const Pawn = (APlayerRocket*)GetPawn();
	
	//const FVector Torque = FVector(0.f, 0.f, 1.f * val * TorqueForce);
	//const FVector Force = Pawn->GetActorForwardVector() * TorqueForce * val;
	rotationVal += rotationSpeed * val;
	FRotator Turn = FRotator(0.f, rotationVal, 0.f);
	Pawn->SetActorRotation(Turn);
	
	//Pawn->GetRocket()->AddForce(Force);
}

void APlayerRocketController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Moving to new Location");
	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Moving to new Location");
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void APlayerRocketController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const Pawn = GetPawn();
	if (Pawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 10.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APlayerRocketController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Left Mouse Pressed");
	bMoveToMouseCursor = true;
}

void APlayerRocketController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Left Mouse Released");
	bMoveToMouseCursor = false;
}
