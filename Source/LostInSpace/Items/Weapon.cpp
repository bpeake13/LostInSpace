// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "../Rocket/PlayerRocket.h"
#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
	ItemName = TEXT("Weapon");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMesh(TEXT("/Game/RocketMesh.RocketMesh"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerUp1"));
	Mesh->SetStaticMesh(ItemMesh.Object);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->bGenerateOverlapEvents = true;
	CollisionComponent->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::ItemPickup()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Weapon Picked Up");
	if (Mesh){
		Destroy();
	}
}

void AWeapon::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerRocket* player = Cast<APlayerRocket>(OtherActor);
	if (player){
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Weapon Collided with Player");
	}
}

