
// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Wandering.h"

void UWandering::Tick(UHierarchicalStateMachine* machine){

}

void UWandering::Execute(UHierarchicalStateMachine* machine){
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "HSM:Wander Tick");

	//Get Owner from HSM and cast it to Owner's class
	AAIRocket* pursuer = Cast<AAIRocket>(machine->GetOwnerActor());

	//Get the locations of the player and the Rocket
	FVector pursuerLoc = pursuer->GetActorLocation();

	cicleRadius = 5;
	circleDistance = 5;
	randAngleVal = FMath::FRandRange(PI, PI * 2);
	
	wanderAngle = FVector(FMath::Cos(randAngleVal), FMath::Sin(randAngleVal), 0.f);
	circleCenter = FVector(pursuerLoc.X + circleDistance, pursuerLoc.Y + circleDistance, 0.f);
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Wander Angle: " + wanderAngle.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Circle Center: " + circleCenter.ToString());
	circleCenter *= wanderAngle;
	//Apply random angle to movement direction

	//Caculate Seek vector
	FVector diff = circleCenter - pursuerLoc;
	diff.Normalize();

	//Apply force to Rocket
	pursuer->GetRocket()->SetRelativeRotation(diff.Rotation());
	pursuer->GetRocket()->AddForce(diff * pursuer->MovementSpeed);
}



