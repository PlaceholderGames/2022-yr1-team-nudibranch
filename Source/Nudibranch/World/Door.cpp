// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //Create a scene component as the root - use this as a root as it won't rotate
	Position = CreateDefaultSubobject<USceneComponent>(TEXT("Position Comp"));
	RootComponent = Position;

    const FVector CollSize = {4.0f, 45.0f, 100.0f}; //size of the doors collision
    const FVector CollLoc = {0.0f, -45.0f, 100.0f}; //position of the box collider in relation to the mesh

    //Create static mesh
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    DoorMesh->SetupAttachment(Position);
    //DoorMesh->SetRelativeLocation(MeshLoc);

    //Create box collider
    DoorColl = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Collision"));
	DoorColl->SetupAttachment(DoorMesh); //attach the collider to the mesh to use the meshes rotation
	DoorColl->InitBoxExtent(CollSize);
    DoorColl->SetRelativeLocation(CollLoc);
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

//Open the door // Rotate 90 degrees
void ADoor::OpenDoor()
{
	if (bIsClosed)
	{
		bIsClosed = false;
		bIsOpen = true;

        //use doormesh for rotation as its pivot point is not centred like the collider is
		FRotator Rotation = FRotator(0.0f, 90.0f, 0.0f);
		DoorMesh->AddRelativeRotation(FQuat(Rotation), false, 0, ETeleportType::None);

        PlayDoorSound();
	}
}

//Close the door // Rotate -90 degrees
void ADoor::CloseDoor()
{
	if (bIsOpen)
	{
		bIsClosed = true;
		bIsOpen = false;

		FRotator Rotation = FRotator(0.0f, -90.0f, 0.0f);
        DoorMesh->AddRelativeRotation(FQuat(Rotation), false, 0, ETeleportType::None);

        PlayDoorSound();
	}
}

void ADoor::PlayDoorSound()
{
    FVector DoorLocation = DoorMesh->GetComponentLocation();
    APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorSound, DoorLocation, ADoor::GetActorRotation(), 0.2f, 1.0f, 0.0f);

    ADoor::MakeNoise(0.2f, PlayerCharacter, DoorLocation, 0.0f, "DoorSound");
}