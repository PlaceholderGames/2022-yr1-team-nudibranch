// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSEnemy.generated.h"

UCLASS()
class NUDIBRANCH_API AFPSEnemy : public ACharacter
{
	GENERATED_BODY()

// ****** DEFAULT UE STUFF ******

public:
	// Sets default values for this character's properties
	AFPSEnemy();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// ******************************


public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* damageCollision;

	UFUNCTION()
		void onHit(UPrimitiveComponent* hitComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodyIndex,
			bool bFromSweep, const FHitResult& hit);

};
