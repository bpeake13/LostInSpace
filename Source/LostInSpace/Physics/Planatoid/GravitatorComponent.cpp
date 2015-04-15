// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "GravitatorComponent.h"


// Sets default values for this component's properties
UGravitatorComponent::UGravitatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;

	GravityStrength = 980.f;
	// ...
}


void UGravitatorComponent::GetUpAtPoint(const FVector& point, FVector& o_upVector)
{
	UWorld* world = GetWorld();
	if (!world)
		return;

	FVector end;
	CalculateUpCheckPoint(point, end);

	FCollisionQueryParams queryParams;
	queryParams.bTraceComplex = false;

	FHitResult groundResult;
	if (!world->LineTraceSingle(groundResult, point, end, ECollisionChannel::ECC_GameTraceChannel1, queryParams))
		return;

	o_upVector = groundResult.ImpactNormal;
}

void UGravitatorComponent::CalculateUpCheckPoint(const FVector& point, FVector& outEndPoint)
{
	outEndPoint = FVector(point.X, point.Y, GetComponentLocation().Z);
}


// Called when the game starts
void UGravitatorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	TArray<USceneComponent*> children;
	GetChildrenComponents(true, children);

	for (TArray<USceneComponent*>::TIterator i = children.CreateIterator(); i; i++)
	{
		USceneComponent* sc = *i;
		UPrimitiveComponent* pc = Cast<UPrimitiveComponent>(sc);
		if (!pc)
			continue;

		affectorZones.Add(pc);

		pc->bGenerateOverlapEvents = true;
		pc->bMultiBodyOverlap = false;
		
		pc->OnComponentBeginOverlap.AddDynamic(this, &UGravitatorComponent::OnBeginOverlap);
		pc->OnComponentEndOverlap.AddDynamic(this, &UGravitatorComponent::OnEndOverlap);
	}
}

void UGravitatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	for (UPlanatoidDataComponent* actor : affectedActors)
	{
		FVector up;
		GetUpAtPoint(actor->GetOwner()->GetActorLocation(), up);

		actor->SetUpVector(up);
	}
}

void UGravitatorComponent::OnBeginOverlap(AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor || OtherActor == this->GetOwner())
		return;

	UPlanatoidDataComponent* planatoidData = Cast<UPlanatoidDataComponent>(OtherActor->GetComponentByClass(UPlanatoidDataComponent::StaticClass()));
	if (!planatoidData)
		return;

	planatoidData->SetCurrentGravitator(this);
}

void UGravitatorComponent::OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this->GetOwner())
		return;

	UPlanatoidDataComponent* planatoidData = Cast<UPlanatoidDataComponent>(OtherActor->GetComponentByClass(UPlanatoidDataComponent::StaticClass()));
	if (!planatoidData)
		return;

	RemoveAffector(planatoidData);
}

void UGravitatorComponent::AddAffector(UPlanatoidDataComponent* affector)
{
	if (!affectedActors.Contains(affector))
		affectedActors.Add(affector);
}

void UGravitatorComponent::RemoveAffector(UPlanatoidDataComponent* affector)
{
	if (affectedActors.Contains(affector))
		affectedActors.Remove(affector);
}