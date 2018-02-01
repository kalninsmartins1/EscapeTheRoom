// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDoorRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEBUILDING_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void OpenDoor();
	void CloseDoor();
	float GetCurrentMassOnPlate() const;


	UPROPERTY(VisibleAnywhere)
	float OpenAngle;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;
	
	UPROPERTY(EditAnywhere)
	float TimeBeforeDoorCloses;

	UPROPERTY(EditAnywhere)
	float MassToTriggerDoors;

	UPROPERTY(BlueprintCallable)
	FOpenDoorRequest OpenDoorRequest;

	AActor* Owner;

	float LastTimeDoorWasOpened;
	bool bIsDoorOpened;
};
