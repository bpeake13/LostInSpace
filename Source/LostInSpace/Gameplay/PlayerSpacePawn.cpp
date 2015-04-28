// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PlayerSpacePawn.h"


APlayerSpacePawn::APlayerSpacePawn()
{
	InputAcceleration = 500.f;
}

void APlayerSpacePawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Horizontal", this, &APlayerSpacePawn::OnMoveHorizontal);
	InputComponent->BindAxis("Vertical", this, &APlayerSpacePawn::OnMoveVertical);
	InputComponent->BindAxis("HorizontalAlt", this, &APlayerSpacePawn::OnRotate);
}

void APlayerSpacePawn::OnMoveVertical(float val)
{
	check(MovementComponent);

	MovementComponent->AddAcceleration(InputAcceleration * val * FVector::ForwardVector);
}

void APlayerSpacePawn::OnMoveHorizontal(float val)
{
	check(MovementComponent);

	MovementComponent->AddAcceleration(InputAcceleration * val * FVector(0, 1, 0));
}

void APlayerSpacePawn::OnRotate(float val)
{
	check(MovementComponent);


}