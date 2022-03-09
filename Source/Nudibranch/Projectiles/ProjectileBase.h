// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class NUDIBRANCH_API AProjectileBase : public AActor
{
	GENERATED_BODY()

// ****** DEFAULT UE STUFF ******
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
// ******************************;

public:
	//UPROPERTY(EditAnywhere)
		//class USkeletalMeshComponent* projectileMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* collSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		class UProjectileMovementComponent* projectileMovement;

    UPROPERTY(EditAnywhere)
        class USoundBase* hitSound;

	UFUNCTION()
		void onHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector impluse, const FHitResult& hit);

	UPROPERTY(EditAnywhere)
		float damageVal = 20.0f;


protected:
    void PlayHitSound();
};
