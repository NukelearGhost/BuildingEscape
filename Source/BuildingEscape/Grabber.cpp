// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	// Check for and configure the Input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Input component for %s"), *GetOwner()->GetName());
	}

	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

void UGrabber::FindPhysicsHandle()
{
	//Check for Physics Handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle component for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	//Ray-cast when the player tries to grab something
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	FVector LineTraceEnd = GetPlayerReach();
	AActor *ActorHit = HitResult.GetActor();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)
		{
			return;
		}
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = GetPlayerReach();
	// If the physics handle is attached
	if (!PhysicsHandle)
	{
		return;
	}
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	// Ray-cast out to a certain distance (reach)
	FHitResult Hit;
	FPlayerViewPoint PlayerViewPoint = GetPlayerViewPoint();

	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPoint.Location,
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	// See what hits, is it something player can pick up?
	AActor *ActorHit = Hit.GetActor();

	return Hit;
}

FVector UGrabber::GetPlayerReach() const
{
	FPlayerViewPoint PlayerViewPoint = GetPlayerViewPoint();
	return PlayerViewPoint.Location + PlayerViewPoint.Rotation.Vector() * Reach;
}

FPlayerViewPoint UGrabber::GetPlayerViewPoint() const
{
	FPlayerViewPoint PlayerViewPoint;
	// Get the players viewpoint

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint.Location,
		OUT PlayerViewPoint.Rotation);

	return PlayerViewPoint;
}