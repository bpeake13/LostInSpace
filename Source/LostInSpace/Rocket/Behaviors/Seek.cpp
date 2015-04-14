// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Seek.h"

void USeek::Tick(UHierarchicalStateMachine* machine){

}

void USeek::Execute(UHierarchicalStateMachine* machine){
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "HSM: Seeking");
	
	//Get Owner from HSM and cast it to Owner's class
	AAIRocket* pursuer = Cast<AAIRocket>(machine->GetOwnerActor());
	
	//Get the locations of the player and the Rocket
	FVector playerLoc = pursuer->PlayerLocation;
	FVector pursuerLoc = pursuer->GetActorLocation();

	//Caculate Seek vector
	FVector diff = playerLoc - pursuerLoc;
	diff.Normalize();

	//Apply force to Rocket
	pursuer->GetRocket()->AddForce(diff * pursuer->MovementSpeed);
	//pursuer->GetRocket()->AddForce(FVector(1, 0, 0) * 500.f);
}

	
	
	
	


