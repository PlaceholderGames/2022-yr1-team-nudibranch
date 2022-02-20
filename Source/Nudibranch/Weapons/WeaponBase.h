// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class NUDIBRANCH_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
// ****** DEFAULT UE STUFF ******
public:
	// Sets default values for this character's properties
	AWeaponBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
// ******************************;

public:

	void startFire();
	void stopFire();
	void reload();

	int getResAmmo();
	int getClipAmmo();

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* weapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* fireAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* fireSound;

	FTimerHandle TimerHandle_ShotTimer;

	float lastFireTime;
	float shotTimer; //firerate

protected:
	class UWorld* world;
	class UAnimInstance* animInstance;

	FRotator projectileRotation;
	FVector projectileLocation;

	//// Weapon Parameters ////

	// Ammo parameters
	UPROPERTY(EditAnywhere)
		int ammoDiff; //ammo reserve

	UPROPERTY(EditAnywhere)
		int clipSize; //size of clip/mag

	UPROPERTY(EditAnywhere)
		int ammoInClip; //currently loaded ammo

	UPROPERTY(EditAnywhere)
		int reserveAmmo; //reserve ammo

	UPROPERTY(EditAnywhere)
		float fireRate;

	void fire();

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		class USceneComponent* muzzleLocation;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AProjectileBase> ProjectileClass;

	class AProjectileBase* projectile;
};
