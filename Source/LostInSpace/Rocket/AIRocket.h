// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostInSpaceClasses.h"

#include "AI/HierarchicalStateMachine.h"
#include "AI/HSMActionNode.h"

#include "GameFramework/Pawn.h"

#include "RocketMovementComponent.h"
#include "PlayerRocket.h"

#include "AIRocket.generated.h"

class USeek;
class UWandering;

UCLASS()
class LOSTINSPACE_API AAIRocket : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rocket, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Rocket;

public:
	// Sets default values for this pawn's properties
	AAIRocket(const FObjectInitializer& ObjectInitializer);

	/** Returns Rocket subobject **/
	class UStaticMeshComponent* GetRocket() const { return Rocket; }
	
	UHierarchicalStateMachine* HSM;
	USeek* Seek;
	UWandering* Wandering;

	//Movement Speed for Rocket
	float MovementSpeed;
	
	//Location of the Player Actor
	APlayerRocket* PlayerActor;
	FVector PlayerLocation;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

};
