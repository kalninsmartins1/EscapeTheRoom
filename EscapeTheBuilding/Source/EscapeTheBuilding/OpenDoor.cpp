// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor():
	OpenAngle(90),
	PressurePlate(nullptr),
	TimeBeforeDoorCloses(1),
	LastTimeDoorWasOpened(0),
	bIsDoorOpened(false),
	MassToTriggerDoors(40),
	Owner(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	

	// Get the owner of this component
	Owner = GetOwner();

	if(PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Grabber: Missing refference to PressurePlate from actor %s"),
			*Owner->GetName());	
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume for key actor overlap
	if(PressurePlate != nullptr &&
		GetCurrentMassOnPlate() >= MassToTriggerDoors &&
		!bIsDoorOpened)
	{
		OpenDoor();	
		LastTimeDoorWasOpened = GetWorld()->GetTimeSeconds();
	}

	// If the Time for door too close has passed then close the doors
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimePassedSinceDoorWasOpen = CurrentTime - LastTimeDoorWasOpened;
	if(bIsDoorOpened && TimePassedSinceDoorWasOpen > TimeBeforeDoorCloses)
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	OpenDoorRequest.Broadcast();

	// Doors are now oppened
	bIsDoorOpened = true;
}

void UOpenDoor::CloseDoor()
{
	const FRotator& CurRotation = Owner->GetActorRotation();

	// Rotate door in positive direction by OpenAngle degrees
	Owner->SetActorRotation(FRotator(CurRotation.Pitch,
		CurRotation.Yaw + OpenAngle, CurRotation.Roll));

	// Doors are now closed
	bIsDoorOpened = false;
}

float UOpenDoor::GetCurrentMassOnPlate() const
{
	float TotalMassOnPlate = 0;

	/* Check that we actually have a 
	 * pressure plate before cauculating total 
	 * mass that lays on it.
	*/
	if(PressurePlate != nullptr)
	{
		// Find all overlapping actors
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		// Sum their mass 
		for (const AActor* Actor : OverlappingActors)
		{
			UPrimitiveComponent* PrimitiveComponent =
				Actor->FindComponentByClass<UPrimitiveComponent>();
			if (PrimitiveComponent != nullptr)
			{
				TotalMassOnPlate += PrimitiveComponent->GetMass();
			}
		}
	}

	return TotalMassOnPlate;
}

