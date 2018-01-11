// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor():
	OpenAngle(90)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume for key actor overlap
	if(PressurePlate != nullptr && ActorThatTriggersDoor != nullptr &&
		PressurePlate->IsOverlappingActor(ActorThatTriggersDoor))
	{		
		OpenDoor();	
	}
	
}

void UOpenDoor::OpenDoor()
{
	// Get owner actor
	AActor* Owner = GetOwner();

	// Get current actor rotation
	const FRotator currentRotation = Owner->GetActorRotation();

	// Rotate actor on yaw axis by 90 degrees
	Owner->SetActorRotation(FRotator(currentRotation.Pitch,
		currentRotation.Yaw - 90, currentRotation.Roll));
}

