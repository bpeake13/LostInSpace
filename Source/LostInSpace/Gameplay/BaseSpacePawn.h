// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BaseSpacePawn.generated.h"

UCLASS(abstract)
class LOSTINSPACE_API ABaseSpacePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseSpacePawn();

protected:
	void InitRoot();
};
