// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Core/Public/Misc/AssertionMacros.h"

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

	FindPhysicsComponent();
	SetUpInput();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		
	// If a component has been grabbed then move it
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->SetTargetLocation(GetMaxReachPosition());
	}
}

void UGrabber::OnGrabPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber: Grab pressed !"));
	TryGrabbing();
}
void UGrabber::OnGrabReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber: Grab released !"));
	Release();
}

void UGrabber::FindPhysicsComponent()
{
	// Get the sibling physics handle component
	AActor* Owner = GetOwner();
	PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		FString ActorName = Owner->GetName();
		UE_LOG(LogTemp, Error,
			TEXT("Grabber: Missing PhysicsHandleComponent from actor %s"),
			*ActorName);
	}
}

void UGrabber::SetUpInput()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber: Input component found !"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::OnGrabPressed);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::OnGrabReleased);
	}
	else
	{
		check(false && "Grabber: InputComponent not found !");
	}
}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	// Perform actual ray cast
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetOwner()->GetActorLocation(),
		GetMaxReachPosition(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));

	return Hit;
}

FVector UGrabber::GetMaxReachPosition() const
{
	// Get view point rotation and player location
	FVector PlayerLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->
		GetPlayerViewPoint(OUT PlayerLocation,
			OUT ViewPointRotation);
	FVector MaxReachPosition = PlayerLocation +
		ViewPointRotation.Vector() * Reach;

	return MaxReachPosition;
}

void UGrabber::TryGrabbing()
{
	FHitResult Hit = GetPhysicsBodyInReach();

	if (Hit.Actor != nullptr)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.Component.Get(),
			"", Hit.Actor->GetActorLocation(), FRotator::ZeroRotator);
	}
}
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

