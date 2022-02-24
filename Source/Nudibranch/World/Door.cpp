// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Position = CreateDefaultSubobject<USceneComponent>(TEXT("Position Comp"));
	RootComponent = Position;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADoor::OpenDoor()
{
	if (bIsClosed)
	{
		bIsClosed = false;
		bIsOpen = true;

		FRotator Rotation = FRotator(0.0f, 90.0f, 0.0f);
		DoorMesh->AddRelativeRotation(FQuat(Rotation), false, 0, ETeleportType::None);
	}
}

void ADoor::CloseDoor()
{
	if (bIsOpen)
	{
		bIsClosed = true;
		bIsOpen = false;

		FRotator Rotation = FRotator(0.0f, -90.0f, 0.0f);
		DoorMesh->AddRelativeRotation(FQuat(Rotation), false, 0, ETeleportType::None);
	}
}


