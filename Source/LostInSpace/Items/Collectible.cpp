// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Collectible.h"


// Sets default values
ACollectible::ACollectible()
{
	ItemName = TEXT("Power Up");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collectible1"));

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));

	CollisionComponent->bGenerateOverlapEvents = true;
	CollisionComponent->BodyInstance.SetCollisionProfileName("ItemCollision");

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlapBegin);

	RootComponent = CollisionComponent;
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();

}

void ACollectible::ItemPickup(){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Power-Up Picked Up");
	if (Mesh){
		Destroy();
	}
}

void ACollectible::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Collision detected!");
	APlayerPlanetPawn* player = Cast<APlayerPlanetPawn>(OtherActor);
	if (player){
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Collectible Collided with Player");
		ItemPickup();
	}
}
