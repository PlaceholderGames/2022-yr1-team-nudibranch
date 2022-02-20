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
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	shotTimer = 60 / fireRate;

	//attach the muzzleLocation to the weapon at the muzzle bone
	muzzleLocation->AttachToComponent(weapMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Muzzle"));//make sure gun skeleton has muzzle bone
	//muzzleLocation->SetRelativeLocation(FVector::ZeroVector); //(0.2f, 48.4f, -10.6f)); //offset from the grip point to the muzzle if not using muzzle bone
}

void AWeaponBase::fire()
{
	AActor* player = GetOwner();
	world = GetWorld();

	if (world != NULL)	//check the world exists
	{
		if (ammoInClip > 0) //Check that the weapon has ammo
		{
			ammoInClip -= 1; //dec ammo count

			FVector eyeLoc; //used to store the location of the players eyes

			//get rotation for projectile
			if (player)
			{
				player->GetActorEyesViewPoint(eyeLoc, projectileRotation); //set the rotation to the same as the eyes
			}
			else
			{
				//not an ideal method will not take into account roll or pitch
				//this will be used if player is not found
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("!owner"));
				projectileRotation = GetActorRotation();
				projectileRotation.Add(0, 90, 0);
			}

			//get location for projectile
			if (muzzleLocation != NULL)
			{
				projectileLocation = muzzleLocation->GetComponentLocation();
			}
			else
			{
				//if muzzle does not exist then use the eye location for spawning the projectile
				projectileLocation = eyeLoc;
			}

			//check if the projectile will collide immediatly, if it does then try to move it
			FActorSpawnParameters actorSpawnParams;
			actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//spawn the projectile
			projectile = world->SpawnActor<AProjectileBase>(ProjectileClass, projectileLocation, projectileRotation, actorSpawnParams);

			//play the firing sound
			if (fireSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());
			}
			
			//play animation
			weapMesh->PlayAnimation(fireAnim, false);
		}
		else //no ammo
		{
			//debug
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clip empty: reloading"));

			stopFire(); //stop firing 
			reload(); //reload
		}
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

//used for hud display
int AWeaponBase::getResAmmo()
{
	return reserveAmmo;
}

//used for hud display
int AWeaponBase::getClipAmmo()
{
	return ammoInClip;
}