// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class NUDIBRANCH_API ADoor : public AActor
{
	GENERATED_BODY()

// ****** DEFAULT UE STUFF ******
public:	
	// Sets default values for this actor's properties
	ADoor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
// ******************************;

public:

	//Object used to reference position
	// - use this as the door mesh will be moving
	class USceneComponent* Position;

    UPROPERTY(VisibleAnywhere)
        class UBoxComponent* DoorColl;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* DoorMesh;

    UPROPERTY(EditAnywhere)
        class USoundBase* DoorSound;

	void OpenDoor();
	void CloseDoor();
protected:

	bool bIsOpen = false;
	bool bIsClosed = true;

    void PlayDoorSound();

};
