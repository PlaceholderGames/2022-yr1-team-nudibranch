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

	UPROPERTY(EditAnywhere, meta = (DisplayName="Time to stay open"))
		float openTime;

	FTimerHandle TimerHandle_DoorTimer;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

    //Play ticking sound to let the player know the button is pressed.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlaySoundFunction")
        void PlayTickSound();

protected:

	void CloseDoor();

    bool isOpen = false;

};
