// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clock.generated.h"

UCLASS()
class NUDIBRANCH_API AClock : public AActor
{
	GENERATED_BODY()

// ****** DEFAULT UE STUFF ******
public:
    // Sets default values for this actor's properties
    AClock();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
// ******************************;

public:

    UPROPERTY(EditAnywhere)
        class UStaticMeshComponent* ClockMesh;

    UPROPERTY(EditAnywhere)
        class UStaticMeshComponent* SecondHandMesh;

    UPROPERTY(EditAnywhere)
        class UStaticMeshComponent* MinuteHandMesh;

    UPROPERTY(EditAnywhere)
        class UStaticMeshComponent* HourHandMesh;

    UPROPERTY(EditAnywhere)
        class USoundBase* TickSound;

private:

    FDateTime CurrentTime;

    FTimerHandle TimerHandle_SecondTimer;
    FTimerHandle TimerHandle_MinuteTimer;
    FTimerHandle TimerHandle_HourTimer;

    void SecondElapsed();
    void PlayTickSound();
};
