// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM()
enum EnumFireMode
{
	AUTO	UMETA(DisplayName = "Fully Automatic"),
	SEMI	UMETA(DisplayName = "Semi Automatic / Single Fire"),
	BURST	UMETA(DisplayName = "Burst Fire")
};

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
    bool getInfAmmo();
    bool getInfResAmmo();

	//Weapon mesh
	UPROPERTY(EditAnywhere, Category = Weapon)
		class USkeletalMeshComponent* weapMesh;

	//animation used when firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class UAnimMontage* fireAnim;

	//sound used when firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USoundBase* fireSound;

	FTimerHandle TimerHandle_ShotTimer;

	float lastFireTime;
	float shotTimer; //firerate

    USceneComponent* getMuzzleLocation(){
        return muzzleLocation();
    }

protected:
	class UWorld* world;
	class UAnimInstance* animInstance;

	FRotator projectileRotation;
	FVector projectileLocation;

	int numShots;

	//// Weapon Parameters ////

	// Ammo parameters
	int ammoDiff; //diff between loaded ammo and clip size

    UPROPERTY(EditAnywhere, Category = Weapon, meta = (DisplayName="Infinite Clip Ammo?"))
        bool bInfClipAmmo;

    UPROPERTY(EditAnywhere, Category = Weapon, meta = (DisplayName="Infinite Reserve Ammo?"))
        bool bInfResAmmo;

	//size of clip/mag
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (ClampMin=1, DisplayName="Magazine Size"))
		int clipSize;

	//currently loaded ammo
	UPROPERTY(EditAnywhere, Category = Weapon)
		int ammoInClip;

	//reserve ammo
	UPROPERTY(EditAnywhere, Category = Weapon)
		int reserveAmmo;

	//Rate of fire
	UPROPERTY(EditAnywhere, Category = Weapon)
		float fireRate;

	//Firing mode that the weapon will use
	UPROPERTY(EditAnywhere, Category = Weapon /*meta = (DisplayName = "Firing Mode")*/)
		TEnumAsByte<EnumFireMode> eFireMode;
		
	void fire();
	void burstFire(int &burstCount);

	//Location the projectile will shoot from
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		class USceneComponent* muzzleLocation;

	//Projectile that the weapon will shoot
	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<class AProjectileBase> ProjectileClass;

    //Declared in blueprint to allow for Vision sphere spawning
    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlaySoundFunction")
    void playFireSound();

	class AProjectileBase* projectile;
};
