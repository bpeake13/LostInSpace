// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "SpaceNavPointActor.h"


// Sets default values
ASpaceNavPointActor::ASpaceNavPointActor()
{
	NavComponent = CreateDefaultSubobject<USpaceNavPoint>(TEXT("NavComponent"));
	NavComponent->SetRelativeLocation(FVector(0, 0, 0));

	RootComponent = NavComponent;

#if WITH_EDITOR

	

#endif
	
}

float ASpaceNavPointActor::GetNavPointRadius() const
{
	return NavComponent->GetRadius();
}

void ASpaceNavPointActor::SetNavPointRadius(float val)
{
	NavComponent->SetRadius(val);
}
