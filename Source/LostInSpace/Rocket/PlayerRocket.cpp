// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "PlayerRocket.h"


// Sets default values
APlayerRocket::APlayerRocket(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RocketMesh(TEXT("/Game/RocketMesh.RocketMesh"));
	
	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket1"));
	Rocket->SetStaticMesh(RocketMesh.Object);
	Rocket->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Rocket->SetSimulatePhysics(true);
	Rocket->SetEnableGravity(false);
	Rocket->SetAngularDamping(0.1f);
	Rocket->SetLinearDamping(0.1f);
	Rocket->BodyInstance.MassScale = 3.5f;
	Rocket->BodyInstance.MaxAngularVelocity = 800.0f;
	Rocket->SetNotifyRigidBodyCollision(true);
	RootComponent = Rocket;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;	
	//bConstrainToPlane = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}



