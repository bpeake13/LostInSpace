// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RocketMovementComponent.h"
#include "AI/HierarchicalStateMachine.h"
#include "AI/HSMActionNode.h"
#include "GameFramework/Pawn.h"
#include "AIRocket.generated.h"

UCLASS()
class LOSTINSPACE_API AAIRocket : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rocket, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Rocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class URocketMovementComponent* MovementComponent;
public:
	// Sets default values for this pawn's properties
	AAIRocket(const FObjectInitializer& ObjectInitializer);

	/** Returns Rocket subobject **/
	class UStaticMeshComponent* GetRocket() const { return Rocket; }

	void Seek();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

};
