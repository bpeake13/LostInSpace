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
	
	//Init Camera variables
	BaseCameraHeight = 500.f;
	CameraHeight = BaseCameraHeight;
	maxX = 0.f, maxY = 0.f, minX = 0.f, minY = 0.f;
	
	CameraBoom->TargetArmLength = CameraHeight;
	CameraBoom->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
	DetectionBox->SetBoxExtent(FVector(2000.f, 2000.f, 1000.f), true);
	DetectionBox->bGenerateOverlapEvents = true;
	DetectionBox->BodyInstance.SetCollisionProfileName("DetectionField");

	DetectionBox->bAbsoluteRotation = true; //Don't want to rotate the range when the character does
	DetectionBox->ShapeColor = FColor::Red;
	DetectionBox->AttachParent = RootComponent;

	ProjectileClass = AProjectile::StaticClass();
	FireCooldown = 3.f;
	fireCooldownTimer = 3.f;
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

	if (fireCooldownTimer > 0)
		fireCooldownTimer -= DeltaTime;
}

void APlayerPlanetPawn::CheckCamera( float DeltaTime ){
	float deltaTime = this->GetWorld()->GetDeltaSeconds();
	
	if (DetectedEnemies.Num() > 0){
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SocketOffset: " + CameraBoom->SocketOffset.ToString());
		CameraBoom->TargetOffset = FMath::VInterpTo(this->GetActorLocation(), CameraOffset, deltaTime, 2.f);
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, CameraHeight, deltaTime, 2.f);
	}
	else {
		CameraBoom->TargetOffset = FMath::VInterpTo(CameraBoom->TargetOffset, this->GetActorLocation(), deltaTime, 2.f);
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraHeight, 500.f, deltaTime, 2.f);
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

//Calculate the height of the camera based on the objects within the detection radius
float APlayerPlanetPawn::CalculateCameraHeight()
{
	float result = 0.f;
	
	//Used when minX/Y and maxX/Y are obtained;
	float width, height;
	
	if (DetectedEnemies.Num() > 0){
		for (auto Iter(DetectedEnemies.CreateIterator()); Iter; Iter++)
		{
			// *Iter to access what this iterator is pointing to.
			if (maxX < (*Iter)->GetActorLocation().X)
				maxX = (*Iter)->GetActorLocation().X;

			if (maxY < (*Iter)->GetActorLocation().Y)
				maxY = (*Iter)->GetActorLocation().Y;

			if (minX > (*Iter)->GetActorLocation().X)
				minX = (*Iter)->GetActorLocation().X;

			if (minY > (*Iter)->GetActorLocation().Y)
				minY = (*Iter)->GetActorLocation().Y;
		}
	}
	else {
		result = BaseCameraHeight;
		return result;
	}

	width = maxX - minX;
	height = maxY - minY;

	float newWidth, newHeight;

	float const AspectRatio = TopDownCameraComponent->AspectRatio;
	if (width > height){
		newHeight = width / AspectRatio;
		result = (BaseCameraHeight / height) * newHeight;
	}
	else{
		newWidth = height * AspectRatio;
		result = (BaseCameraHeight / width) * newWidth;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Camera Height: " + FString::SanitizeFloat(result));
	return result;
}

// Called to bind functionality to input
void APlayerPlanetPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Vertical", this, &APlayerPlanetPawn::OnVertical);
	InputComponent->BindAxis("Horizontal", this, &APlayerPlanetPawn::OnHorizontal);
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPlanetPawn::OnFire);
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

void APlayerPlanetPawn::OnFire()
{
	FVector actorForwardVector = this->GetActorForwardVector();
	FVector direction = FVector(actorForwardVector.X, actorForwardVector.Y, 0.f);

	
	if (fireCooldownTimer > 0)
		return;
	
	fireCooldownTimer = FireCooldown;

	UWorld* const World = GetWorld();
	UPrimitiveComponent* rootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "FKEY PRESSED");

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FVector FireLocation = rootPrimitive->GetComponentLocation() + direction * (rootPrimitive->Bounds.GetSphere().W + 100.f);

		// spawn the projectile at the muzzle
		AProjectile* const Projectile = World->SpawnActor<AProjectile>(ProjectileClass, FireLocation, direction.Rotation(), SpawnParams);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Projectile Fired!");

	}
}

void APlayerPlanetPawn::OnDetectionEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor Entered!");
	DetectedEnemies.Add(OtherActor);
	CameraOffset = CalculateCameraOffset();
	CameraHeight = CalculateCameraHeight();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Planet Location: " + this->GetActorLocation().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Calculated Vector: " + CameraOffset.ToString());
}

void APlayerPlanetPawn::OnDetectionExit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor Left!");
	if (DetectedEnemies.Num() > 0){
		DetectedEnemies.Remove(OtherActor);
		CameraHeight = CalculateCameraHeight();
	}
}


