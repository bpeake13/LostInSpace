// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PowerUp.h"


// Sets default values
APowerUp::APowerUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemName = TEXT("Power Up");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerUp1"));
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUp::ItemPickup(){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Item Picked Up");
}
