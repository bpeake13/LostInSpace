// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerSpacePawn.h"


APlayerSpacePawn::APlayerSpacePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	InputAcceleration = 500.f;

	CameraLerpSpeed = 10.f;

	ViewExtent = 5000.f;

	MinViewSize = 5000.f;
	ViewMargin = 1000.f;

	MovementComponent->SetMaxSpeed(800.f);
}

void APlayerSpacePawn::BeginPlay()
{
	Super::BeginPlay();

	viewLocation = GetActorLocation() + FVector(0, 0, 5000.f);
	viewTargetLocation = viewLocation;

	TActorIterator<APlayerPlanetPawn> secondPlayerFinder(GetWorld());
	if (secondPlayerFinder)
		secondPlayer = *secondPlayerFinder;
}

void APlayerSpacePawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Horizontal", this, &APlayerSpacePawn::OnMoveHorizontal);
	InputComponent->BindAxis("Vertical", this, &APlayerSpacePawn::OnMoveVertical);
	InputComponent->BindAxis("HorizontalAlt", this, &APlayerSpacePawn::OnRotate);

	InputComponent->BindAxis("Horizontal_2", this, &APlayerSpacePawn::OnMoveHorizontal_PlanetPawn);
	InputComponent->BindAxis("Vertical_2", this, &APlayerSpacePawn::OnMoveVertical_PlanetPawn);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerSpacePawn::OnFireStart);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerSpacePawn::OnFireStop);
}

void APlayerSpacePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

void APlayerSpacePawn::OnMoveVertical_PlanetPawn(float val)
{
	if (!secondPlayer)
		return;

	secondPlayer->OnVertical(val);
}

void APlayerSpacePawn::OnMoveHorizontal_PlanetPawn(float val)
{
	if (!secondPlayer)
		return;

	secondPlayer->OnHorizontal(val);
}

void APlayerSpacePawn::OnRotate(float val)
{
	check(MovementComponent);


}

void APlayerSpacePawn::OnFireStart()
{
	if (secondPlayer)
		secondPlayer->OnFire();
}

void APlayerSpacePawn::OnFireStop()
{

}

void APlayerSpacePawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	UWorld* world = GetWorld();
	if (!world)
		return;

	FCollisionShape shape;
	shape.SetBox(FVector(ViewExtent, ViewExtent, 20.f));

	FCollisionQueryParams queryParams;
	queryParams.bFindInitialOverlaps = true;

	FCollisionObjectQueryParams objQueryParams;
	objQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
	
	TArray<FOverlapResult> foundObjects;
	if (!world->OverlapMulti(foundObjects, GetActorLocation(), FQuat::Identity, shape, queryParams, objQueryParams))
		return;

	FVector min = FVector(ViewExtent, ViewExtent, 0);
	FVector max = FVector(-ViewExtent, -ViewExtent, 0);

	for (FOverlapResult overlap : foundObjects)
	{
		AActor* overlapActor = overlap.GetActor();
		ABaseSpacePawn* spacePawn = Cast<ABaseSpacePawn>(overlapActor);
		if (!spacePawn)
			continue;

		FVector actorLocation = spacePawn->GetActorLocation() - GetActorLocation();
		if (actorLocation.X < min.X)
			min.X = actorLocation.X;
		if (actorLocation.Y < min.Y)
			min.Y = actorLocation.Y;
		if (actorLocation.X > max.X)
			max.X = actorLocation.X;
		if (actorLocation.Y > max.Y)
			max.Y = actorLocation.Y;
	}

	viewTargetLocation = (max + min) * 0.5f + GetActorLocation();

	//calculate the view width using the view radius
	FVector size = max - min;
	float distance = size.Size() + ViewMargin * 2.f;
	float viewHeight = FMath::Max(distance, MinViewSize);
	float viewWidth = OutResult.AspectRatio * viewHeight;

	viewTargetLocation.Z = viewWidth * 0.5f / FMath::Tan(FMath::DegreesToRadians(OutResult.FOV * 0.5f));

	if (!viewLocation.Equals(viewTargetLocation, 1.f))
		viewLocation = FMath::VInterpTo(viewLocation, viewTargetLocation, DeltaTime, CameraLerpSpeed);
	else
		viewLocation = viewTargetLocation;

	FVector direction = -FVector::UpVector;
	FVector right = FVector::CrossProduct(FVector::ForwardVector, direction);

	OutResult.Rotation = FRotationMatrix::MakeFromXY(direction, right).Rotator();
	OutResult.Location = viewLocation;
}
