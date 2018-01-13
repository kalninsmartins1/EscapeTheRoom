// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor():
	OpenAngle(90),
	TimeBeforeDoorCloses(1),
	LastTimeDoorWasOpened(0),
	bIsDoorOpened(false)
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

	// Find the player pawn
	ActorThatTriggersDoor = GetWorld()->GetFirstPlayerController()->GetPawn();

	// Get the owner of this component
	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume for key actor overlap
	if(PressurePlate != nullptr && ActorThatTriggersDoor != nullptr &&
		PressurePlate->IsOverlappingActor(ActorThatTriggersDoor) &&
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
	const FRotator& CurRotation = Owner->GetActorRotation();
	
	// Rotate door in negative direction by OpenAngle degrees
	Owner->SetActorRotation(FRotator(CurRotation.Pitch,
		CurRotation.Yaw - OpenAngle, CurRotation.Roll));

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

