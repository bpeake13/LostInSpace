// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "AIRocket.h"
#include "PlayerRocket.h"


// Sets default values
AAIRocket::AAIRocket(const FObjectInitializer& ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RocketMesh(TEXT("/Game/RocketMesh.RocketMesh"));

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket1"));
	Rocket->SetStaticMesh(RocketMesh.Object);
	Rocket->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Rocket->SetSimulatePhysics(true);
	Rocket->SetEnableGravity(false);
	Rocket->SetAngularDamping(0.1f);
	Rocket->SetLinearDamping(0.1f);
	Rocket->BodyInstance.MassScale = 3.5f;
	Rocket->BodyInstance.MaxAngularVelocity = 800.0f;
	Rocket->SetNotifyRigidBodyCollision(true);
	RootComponent = Rocket;

	MovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AAIRocket::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAIRocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	Seek();
}

void AAIRocket::Seek(){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Seeking");
	APlayerRocket* player = (APlayerRocket*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector playerLoc = player->GetActorLocation();
	FVector botLoc = this->GetActorLocation();
	FVector diff = playerLoc - botLoc;
	diff.Normalize();
	MovementComponent->AddForce(diff * 50000.f);
}


