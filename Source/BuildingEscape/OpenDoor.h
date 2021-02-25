// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	bool bOpenSound = false;
	bool bCloseSound = true;

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor *ActorThatOpens = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.75f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float PressurePlateThresholdMass = 60.f;

	UPROPERTY(EditAnywhere)
	bool bVerticalDoor = false;

	UPROPERTY(EditAnywhere)
	float VerticalOpening;

	UPROPERTY()
	UAudioComponent *AudioComponent = nullptr;

	float CurrentYaw;
	float InitialYaw;

	float InitialPos;
	float CurrentPos;

	float DoorLastOpened = 0.f;
};
