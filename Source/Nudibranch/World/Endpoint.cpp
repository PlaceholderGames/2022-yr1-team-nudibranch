// Fill out your copyright notice in the Description page of Project Settings.


#include "Endpoint.h"
#include "Nudibranch/FPSPlayer.h"
#include "Nudibranch/FPSGameMode.h"

#include "Components/BoxComponent.h"

// Sets default values
AEndpoint::AEndpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSize = { 5, 5, 5 };

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->InitBoxExtent(CollisionSize);

	RootComponent = BoxCollision;
}

// Called when the game starts or when spawned
void AEndpoint::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEndpoint::OnHit);
}

// Called every frame
void AEndpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndpoint::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& hit)
{
	// check if colliding actor is the player and end the level

    AFPSPlayer* Player = Cast<AFPSPlayer>(OtherActor);

    if (Player)
    {
        GameMode->LevelComplete();
    }
}

