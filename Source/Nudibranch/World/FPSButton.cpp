// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSButton.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSButton::AFPSButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetCapsuleSize(10, 40);

	RootComponent = CapsuleComp;

	DoorComponent = CreateDefaultSubobject<ADoor>(TEXT("Door Component"));

	openTime = 5.0f;

}

// Called when the game starts or when spawned
void AFPSButton::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSButton::OnHit);

}

// Called every frame
void AFPSButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSButton::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	//open door

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPEN THE DOOR"));
	GetWorldTimerManager().SetTimer(TimerHandle_DoorTimer, this, &AFPSButton::CloseDoor, openTime, false, -1.0f);
}

void AFPSButton::CloseDoor()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CLOSE THE DOOR"));
	GetWorldTimerManager().ClearTimer(TimerHandle_DoorTimer);
}