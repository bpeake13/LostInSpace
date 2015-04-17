// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PowerUp.h"


// Sets default values
APowerUp::APowerUp()
{
	ItemName = TEXT("Power Up");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMesh(TEXT("/Game/RocketMesh.RocketMesh"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerUp1"));
	Mesh->SetStaticMesh(ItemMesh.Object);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	
	CollisionComponent->bGenerateOverlapEvents = true;
	CollisionComponent->BodyInstance.SetCollisionProfileName("ItemCollision");

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlapBegin);
	
	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();

}

void APowerUp::ItemPickup(){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Power-Up Picked Up");
	if(Mesh){
		Destroy();
	}
}

void APowerUp::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Collision detected!");
	APlayerRocket* player = Cast<APlayerRocket>(OtherActor);
	if (player){
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Power-Up Collided with Player");
	}
}
