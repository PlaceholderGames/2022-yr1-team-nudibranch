// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "GameFramework/Actor.h"
#include "FPSButton.generated.h"

UCLASS()
class NUDIBRANCH_API AFPSButton : public AActor
{
	GENERATED_BODY()

// ****** DEFAULT UE STUFF ******
public:	
	// Sets default values for this actor's properties
	AFPSButton();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
// ******************************;

public:

	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ButtonMesh;

	UPROPERTY(EditAnywhere, meta = (DisplayName="Door to open"))
		class ADoor* DoorComponent;

    //Sound to play when the door is open
    UPROPERTY(EditAnywhere)
        class USoundBase* SoundComponent;

	UPROPERTY(EditAnywhere, meta = (DisplayName="Time to stay open"))
		float openTime;

    //Play ticking sound to let the player know the button is pressed.
   // UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlaySoundFunction")
        //void PlayTickSound();

protected:

    FTimerHandle TimerHandle_DoorTimer;
    FTimerHandle TimerHandle_DoorSoundTimer;

    void PlayTickSound();
	void CloseDoor();

    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
};
