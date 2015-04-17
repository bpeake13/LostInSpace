// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ItemInterface.generated.h"

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()

};

class IItemInterface
{
	GENERATED_IINTERFACE_BODY()
	
	FString ItemName;

	class UStaticMeshComponent* Mesh;
	class UBoxComponent* CollisionComponent;
	
	UFUNCTION()
	virtual void ItemPickup();

	UFUNCTION()
	virtual void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
