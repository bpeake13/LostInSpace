// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "ItemInterface.h"


UItemInterface::UItemInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

//This is required for compiling, would also let you know if somehow you called
//the base event/function rather than the over-rided version
void IItemInterface::ItemPickup()
{
	
}

void IItemInterface::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
