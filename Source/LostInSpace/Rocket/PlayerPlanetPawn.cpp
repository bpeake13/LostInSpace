// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "Physics/Planatoid/PawnPlanatoidMovementComponent.h"
#include "PlayerPlanetPawn.h"


// Sets default values
APlayerPlanetPawn::APlayerPlanetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	PlayerCapsule->SetCapsuleHalfHeight(40.f);
	PlayerCapsule->SetCapsuleRadius(10.f);
	PlayerCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = PlayerCapsule;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Mesh->SetRelativeLocation(FVector(0, 0, 0));
	Mesh->AttachParent = RootComponent;

	PlanatoidMovement = CreateDefaultSubobject<UPawnPlanatoidMovementComponent>(TEXT("PlanetMovement"));

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 700.0f;
	CameraBoom->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
	DetectionBox->SetBoxExtent(FVector(1000.f, 1000.f, 500.f), true);
	DetectionBox->bGenerateOverlapEvents = true;
	DetectionBox->BodyInstance.SetCollisionProfileName("DetectionField");

	DetectionBox->bAbsoluteRotation = true; //Don't want to rotate the range when the character does
	DetectionBox->ShapeColor = FColor::Red;
	DetectionBox->AttachParent = RootComponent;
}

// Called when the game starts or when spawned
void APlayerPlanetPawn::BeginPlay()
{
	Super::BeginPlay();
	
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerPlanetPawn::OnDetectionEnter);
	DetectionBox->OnComponentEndOverlap.AddDynamic(this, &APlayerPlanetPawn::OnDetectionExit);
}

// Called every frame
void APlayerPlanetPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	CheckCamera(DeltaTime);
}

void APlayerPlanetPawn::CheckCamera( float DeltaTime ){
	float deltaTime = this->GetWorld()->GetDeltaSeconds();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SocketOffset: " + CameraBoom->SocketOffset.ToString());
	if (DetectedEnemies.Num() > 0){
		CameraBoom->TargetOffset = FMath::VInterpTo(this->GetActorLocation(), CameraOffset, deltaTime, 100.f);
		CameraBoom->TargetArmLength = FMath::FInterpTo(700.f, 150.0f, deltaTime, 2);
	}
	else {
		CameraBoom->TargetOffset = FMath::VInterpTo(CameraBoom->TargetOffset, this->GetActorLocation(), deltaTime, 100.f);
	}
	
}

// We want to take the average position of all the entities within the detection radius
FVector APlayerPlanetPawn::CalculateCameraOffset()
{
	FVector result = FVector::ZeroVector;
	int32 numEnemies = DetectedEnemies.Num();

	//If the array of detected enemies is not empty, then iterate and create an average of the positions
	if (DetectedEnemies.Num() > 0){
		for (auto Iter(DetectedEnemies.CreateIterator()); Iter; Iter++)
		{
			// *Iter to access what this iterator is pointing to.
			result += (*Iter)->GetActorLocation();
		}
		result = result / numEnemies;
	}
	return result;
}

// Called to bind functionality to input
void APlayerPlanetPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Vertical", this, &APlayerPlanetPawn::OnVertical);
	InputComponent->BindAxis("Horizontal", this, &APlayerPlanetPawn::OnHorizontal);
	Super::SetupPlayerInputComponent(InputComponent);
}

void APlayerPlanetPawn::OnVertical(float val)
{
	FVector up = PlanatoidMovement->GetUp();
	
	//how much of our up is going towards the planets up
	float planetUpDot = FVector::DotProduct(up, FVector(1, 0, 0));

	float rotationDegrees = FMath::RadiansToDegrees(FMath::Asin(planetUpDot));
	FRotator directionRotator = FRotator(-rotationDegrees, 0, 0);
	FVector moveDirection = directionRotator.RotateVector(FVector::ForwardVector);

	PlanatoidMovement->AddInputVector(moveDirection * val);
}

void APlayerPlanetPawn::OnHorizontal(float val)
{
	FVector up = PlanatoidMovement->GetUp();

	//how much of our up is going towards the planets up
	float planetUpDot = FVector::DotProduct(up, FVector(0, 1, 0));

	float rotationDegrees = FMath::RadiansToDegrees(FMath::Asin(planetUpDot));
	FRotator directionRotator = FRotator();
	FVector moveDirection = directionRotator.RotateVector(FVector(0, 1, 0));

	PlanatoidMovement->AddInputVector(moveDirection * val);
}

void APlayerPlanetPawn::OnDetectionEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor Entered!");
	DetectedEnemies.Add(OtherActor);
	CameraOffset = CalculateCameraOffset();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Planet Location: " + this->GetActorLocation().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Calculated Vector: " + CameraOffset.ToString());
}

void APlayerPlanetPawn::OnDetectionExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor Left!");
	if (DetectedEnemies.Num() > 0){
		DetectedEnemies.Remove(OtherActor);
	}
}


