// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Nudibranch/Projectiles/ProjectileBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	weapMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	RootComponent = weapMesh;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));

	clipSize = 30;
	reserveAmmo = 100;
	ammoInClip = clipSize;

	fireRate = 600;

	gunOffset = FVector(100.0f, 0.0f, 10.0f);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	shotTimer = 60 / fireRate;

	muzzleLocation->AttachToComponent(weapMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Muzzle"));//make sure gun skeleton has muzzle bone
	muzzleLocation->SetRelativeLocation(FVector::ZeroVector); //(0.2f, 48.4f, -10.6f)); //offset from the grip point to the muzzle if not using muzzle bone
}

void AWeaponBase::fire()
{
	if (ammoInClip > 0)
	{
		ammoInClip -= 1; //dec ammo count
		
		////// do other params here //////

		world = GetWorld();

		if (world != NULL) //check the world exists
		{
			projectileRotation = GetActorRotation(); //not sure about this | could try getOwner() first
			projectileRotation.Add(0, 90, 0);

			//check muzzle is not null - if it is then use actor location
			//spawnLocation = ((muzzleLocation != nullptr) ? muzzleLocation->GetComponentLocation() : GetActorLocation()) + spawnRotation.RotateVector(gunOffset);

			if (muzzleLocation != nullptr)
			{
				//debug
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Muzzle not NULL"));
				projectileLocation = muzzleLocation->GetComponentLocation();
			}
			else
			{
				projectileLocation = GetActorLocation(); //+ projectileRotation.RotateVector(gunOffset);
			}

			//check if the projectile will collide immediatly, if it does then try to move it
			FActorSpawnParameters actorSpawnParams;
			actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//spawn the projectile
			projectile = world->SpawnActor<AProjectileBase>(ProjectileClass, projectileLocation, projectileRotation, actorSpawnParams);

			//debug
			if (GEngine && projectile != NULL)
			{
				//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Projectile Spawned"));
			}

			//play the firing sound
			if (fireSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());
			}
			

			//play animation
			weapMesh->PlayAnimation(fireAnim, false);
		}

	}
	else
	{
		//debug
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clip empty: reloading"));

		stopFire(); //stop firing 
		reload(); //reload
	}
}

void AWeaponBase::startFire()
{
	float firstDelay = FMath::Max(lastFireTime + shotTimer - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_ShotTimer, this, &AWeaponBase::fire, shotTimer, true, firstDelay);
}

void AWeaponBase::stopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ShotTimer);
}

void AWeaponBase::reload()
{
	//calc how many bullets until a full clip
	ammoDiff = clipSize - ammoInClip;

	if (reserveAmmo == 0)
	{
		//out of ammo

		//debug
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NO AMMO LEFT"));
		
		//play a sfx & display on screen possibly

		return; //exit reload method
	}

	//check that ammo in clip won't exceed clipsize
	if (ammoInClip < clipSize)
	{
		//reload

		//if there is not enough ammo to fill the clip then fill as much as possible
		if (reserveAmmo <= ammoDiff)
		{
			ammoInClip += reserveAmmo;
			reserveAmmo = 0;
		}
		else
		{
			ammoInClip += ammoDiff;
			reserveAmmo -= ammoDiff;
		}

		//debug
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reloaded"));

		//play reloading sfx hereee
	}
}

//will be used to for HUD
int AWeaponBase::getResAmmo()
{
	return reserveAmmo;
}

//will be used to for HUD
int AWeaponBase::getClipAmmo()
{
	return ammoInClip;
}