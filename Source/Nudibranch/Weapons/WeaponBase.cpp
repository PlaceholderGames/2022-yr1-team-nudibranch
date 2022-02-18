// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	weapMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	RootComponent = weapMesh;

	clipSize = 30;
	reserveAmmo = 100;
	ammoInClip = clipSize;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::fire()
{
	if (ammoInClip > 0)
	{
		ammoInClip -= 1; //dec ammo count
		
		////// do other params here //////
	}
	else
	{
		//debug
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clip empty: reloading"));

		reload();
	}

	//play animation
	weapMesh->PlayAnimation(fireAnim, false);
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

	//check that currently loaded ammo is 0 or > and check that ammo in clip doesn't exceed clipsize
	if (ammoInClip >= 0 && ammoInClip < clipSize)
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

		//play sfx hereee
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