// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PlanatoidDataComponent.generated.h"


UCLASS( ClassGroup=("Planatoid"), meta=(BlueprintSpawnableComponent) )
class UPlanatoidDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlanatoidDataComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetUpVector(const FVector& up);

	UFUNCTION(BlueprintCallable, Category = "Physics")
	FVector GetUpVector() const;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	FVector GetForwardVector() const;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	FVector GetRightVector() const;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	FMatrix GetOrientationMatrix() const;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	UGravitatorComponent* GetCurrentGravitator() const;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetCurrentGravitator(UGravitatorComponent* gravitator);
private:
	FVector up;
	FVector right;
	FVector forward;

	FMatrix orientationMatrix;

	UGravitatorComponent* currentAffector;
};
