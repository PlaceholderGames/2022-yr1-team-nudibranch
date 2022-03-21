// Fill out your copyright notice in the Description page of Project Settings.


#include "Clock.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AClock::AClock()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
    RootComponent = ClockMesh;

    SecondHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondHandComponent"));
    SecondHandMesh->SetupAttachment(ClockMesh);
    MinuteHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinuteHandComponent"));
    MinuteHandMesh->SetupAttachment(ClockMesh);
    HourHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HourHandComponent"));
    HourHandMesh->SetupAttachment(ClockMesh);
}

// Called when the game starts or when spawned
void AClock::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_SecondTimer, this, &AClock::SecondElapsed, 1.0f, true, -1.0f);
}

// only play the tick in the second hand otherwise it will play multiple times.

void AClock::SecondElapsed()
{
    float HourMultiplier = 30.0f;
    float MinuteMultiplier = 6.0f;
    float SecondMultiplier = 6.0f;

    CurrentTime = FDateTime::Now();
    
	float hourRotation = CurrentTime.GetHour() * HourMultiplier;
    float minuteRotation = CurrentTime.GetMinute() * MinuteMultiplier;
    float secondRotation = CurrentTime.GetSecond() * SecondMultiplier;

    HourHandMesh->SetRelativeRotation(FRotator(0.0f, hourRotation, 0.0f));
    MinuteHandMesh->SetRelativeRotation(FRotator(0.0f, minuteRotation, 0.0f));
    SecondHandMesh->SetRelativeRotation(FRotator(0.0f, secondRotation, 0.0f));

    PlayTickSound();
}

void AClock::PlayTickSound()
{
    if (TickSound != nullptr)
    {
    	FVector ClockLocation = ClockMesh->GetComponentLocation();
		APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TickSound, ClockLocation, AClock::GetActorRotation(), 0.2f, 1.0f, 0.0f);

		AClock::MakeNoise(0.5f, PlayerCharacter, ClockLocation, 0.0f, "ClockTickSound");   
    }
}