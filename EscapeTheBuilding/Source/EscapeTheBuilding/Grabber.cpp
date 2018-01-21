// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() :
	Reach(100),
	PhysicsHandle(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Get the sibling physics handle component
	AActor* Owner = GetOwner();
	PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != nullptr)
	{
		// Everything is fine
	}
	else
	{
		FString ActorName = Owner->GetName();
		UE_LOG(LogTemp, Error,
			TEXT("Grabber: Missing PhysicsHandleComponent from actor %s"),
			*ActorName);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Get view point rotation and player location
	FVector PlayerLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, 
		OUT ViewPointRotation);


	// Debug ray cast
	FVector LineEnd = PlayerLocation + ViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerLocation, LineEnd, 
		FColor::Red, false, 0, 0, 10);


	// Perform actual ray cast
	FHitResult Hit;
	const bool bDidHitOccured = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		LineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));
	
	if(bDidHitOccured)
	{
		FString ActorName = Hit.Actor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Grabber: Ray hit %s"), *ActorName);
	}	
}

