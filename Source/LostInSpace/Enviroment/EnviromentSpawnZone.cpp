// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "EnviromentSpawnZone.h"


// Sets default values for this component's properties
UEnviromentSpawnZone::UEnviromentSpawnZone()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;

	SpawnType = NULL;
	SpawnRadius = 1000.f;
	ScanDistance = 200.f;
}

// Called when the game starts
void UEnviromentSpawnZone::InitializeComponent()
{
#if WITH_EDITOR
	if (GIsEditor && FApp::IsGame())
		SpawnInstances();
#else
	SpawnInstances();
#endif // WITH_EDITOR

	Super::InitializeComponent();
}

void UEnviromentSpawnZone::SpawnInstances()
{
	UWorld* world = GetWorld();
	if (!world)
		return;

	FRotator rotation = GetComponentRotation();
	FVector up = rotation.RotateVector(FVector::UpVector);

	FVector startPoint = GetComponentLocation();
	FVector endPoint = startPoint + -up * ScanDistance;

	FCollisionQueryParams params;
	params.bReturnPhysicalMaterial = true;
	params.bFindInitialOverlaps = false;

	FHitResult hit;
	if (!world->LineTraceSingle(hit, startPoint, endPoint, ECollisionChannel::ECC_GameTraceChannel1, params))
		return;

	FVector spawnPoint = hit.ImpactPoint;
	FVector spawnUp = hit.ImpactNormal;

	for (int32 i = 0; i < SpawnCount; i++)
	{
		FVector randDirection = FMath::VRand();
		float randSize = FMath::FRand() * SpawnRadius;

		FVector sphereCheckPoint = randDirection * randSize;
		FVector spawnStartCheckPoint = FVector::PointPlaneProject(sphereCheckPoint, spawnPoint, spawnUp);
		FVector spawnEndCheckPoint = spawnStartCheckPoint - spawnUp * ScanDistance;

		if (!world->LineTraceSingle(hit, startPoint, endPoint, ECollisionChannel::ECC_GameTraceChannel1, params))
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawner %s had a miss spawn, tyring moving closer to surface or changing scan distance."), *GetPathName());
			continue;
		}

		FVector instancePoint = hit.ImpactPoint;
		FVector instanceUp = hit.ImpactNormal;

		FRotator instanceRotation = FRotationMatrix::MakeFromZ(instanceUp).Rotator();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetOwner();
		AActor* instance = world->SpawnActor<AActor>(SpawnType, instancePoint, instanceRotation, spawnParams);

		instance->AttachRootComponentTo(hit.Component.Get(), hit.BoneName, EAttachLocation::KeepWorldPosition);
	}
}

