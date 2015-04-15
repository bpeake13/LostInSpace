// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PlanatoidDataComponent.h"
#include "GravitatorComponent.generated.h"


UCLASS( ClassGroup=("Planatoid"), meta=(BlueprintSpawnableComponent) )
class UGravitatorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravitatorComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	/*Gets the up vector for this gravitator at a certain world point*/
	void GetUpAtPoint(const FVector& point, FVector& o_upVector);

	void AddAffector(UPlanatoidDataComponent* affector);

	void RemoveAffector(UPlanatoidDataComponent* affector);

protected:
	virtual void CalculateUpCheckPoint(const FVector& point, FVector& outEndPoint);
private:
	UFUNCTION()
	void OnBeginOverlap(AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics")
	float GravityStrength;
private:
	TArray<UPrimitiveComponent*> affectorZones;

	TArray<UPlanatoidDataComponent*> affectedActors;
};
