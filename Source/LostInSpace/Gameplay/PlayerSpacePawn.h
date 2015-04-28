// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/BaseSpacePawn.h"
#include "LostInSpaceClasses.h"
#include "Rocket/PlayerPlanetPawn.h"
#include "PlayerSpacePawn.generated.h"

/**
 * 
 */
UCLASS()
class APlayerSpacePawn : public ABaseSpacePawn
{
	GENERATED_BODY()


public:
	APlayerSpacePawn();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BeginPlay() override;



	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION()
	void OnMoveVertical(float val);

	UFUNCTION()
	void OnMoveHorizontal(float val);

	UFUNCTION()
	void OnRotate(float val);

	UFUNCTION()
	void OnMoveVertical_PlanetPawn(float val);

	UFUNCTION()
	void OnMoveHorizontal_PlanetPawn(float val);

	UFUNCTION()
	void OnFireStart();

	void OnFireStop();

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Input")
	float InputAcceleration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "View")
	float CameraLerpSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "View")
	float ViewExtent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "View")
	float MinViewSize;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "View")
	float ViewMargin;
private:
	/*The current view location*/
	FVector viewLocation;

	/*The target for the view*/
	FVector viewTargetLocation;

	APlayerPlanetPawn* secondPlayer;
};
