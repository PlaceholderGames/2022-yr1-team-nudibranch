// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayer.h"

#include "FPSGameMode.h"
#include "Weapons/WeaponBase.h"
#include "Projectiles/ProjectileBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSPlayer::AFPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default capsule(collision) size
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 95.0f);

	//default sensitivity
	turnRate = 45.0f;
	lookUpRate = 45.0f;

	//Create camera and add to the player
	fpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	fpsCamera->SetupAttachment(GetCapsuleComponent());
	fpsCamera->AddRelativeLocation(FVector(-39.65f, 1.75f, 64.0f));
	fpsCamera->bUsePawnControlRotation = true;

	//Create hand mesh
	handsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	handsMesh->SetOnlyOwnerSee(true); //only the player can see their own hands
	handsMesh->SetupAttachment(fpsCamera);
	handsMesh->bCastDynamicShadow = false;
	handsMesh->CastShadow = false;
	handsMesh->AddRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	handsMesh->AddRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	//single weapon code : remove after implementing the weapon base system
	/*Create gun mesh
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;*/

	//Create muzzle component
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));

	//single weapon code : remove after implementing the weapon base system
	/*Create muzzle location(bullet spawn location)
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));*/

	//set offset for the gun
	gunOffset = FVector(100.0f, 0.0f, 10.0f);

}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//set the world to the current world
	world = GetWorld();

	//Check weapon & world exist
	if (WeaponClass && world) //if weapon class exists then add the weapon to the world and attach to the player
	{
		FActorSpawnParameters weapSpawnParams;
		weapSpawnParams.bNoFail = true;
		weapSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform weapTransform;
		weapTransform.SetLocation(FVector::ZeroVector);
		weapTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		//create the weapon
		weap = world->SpawnActor<AWeaponBase>(WeaponClass, weapTransform, weapSpawnParams);

		if (weap) //if weapon is created
		{
			//attach the weapon to the hands
			weap->AttachToComponent(handsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));

			//set the muzzle location here as we need to gun to already exist
			muzzleLocation->AttachToComponent(handsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));//should change this: projectile spawns at hands
			muzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));//::ZeroVector); //(0.2f, 48.4f, -10.6f));

			//debug out
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Created the weapon"));
			}
		}
	}

	//single weapon code : remove after implementing the weapon base system
	//attach the gun to the hands
	//do this here so that skeletal meshes already exist	
	//									  dont scale with hands									socket to attach to
	//GunMesh->AttachToComponent(HandsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));

	//set the anim instance
	animInstance = handsMesh->GetAnimInstance();

}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlayer::onFire);

	//movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlayer::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlayer::moveRight);

	//rotation events
	PlayerInputComponent->BindAxis("Turn", this, &AFPSPlayer::turnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSPlayer::lookUpAtRate);

	//controller rotation events
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFPSPlayer::onFire()
{
	world = GetWorld();

	if (world != NULL) //check the world exists
	{
		spawnRotation = GetControlRotation();

		//check muzzle is not null - if it is then use actor location
		//spawnLocation = ((muzzleLocation != nullptr) ? muzzleLocation->GetComponentLocation() : GetActorLocation()) + spawnRotation.RotateVector(gunOffset);

		if (muzzleLocation != nullptr)
		{
			//debug
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Muzzle not NULL"));
			spawnLocation = muzzleLocation->GetComponentLocation();
		}
		else
		{
			spawnLocation = GetActorLocation() + spawnRotation.RotateVector(gunOffset);
		}

		//check if the projectile will collide immediatly, if it does then try to move it
		FActorSpawnParameters actorSpawnParams;
		actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//TO DO: MAKE THIS SPAWN A WEAPON SPECIFIC PROJECTILE
		//spawn the projectile
		//projectile = 
		world->SpawnActor<AProjectileBase>(ProjectileClass, spawnLocation, spawnRotation, actorSpawnParams);
		
		//debug
		if (GEngine && projectile != NULL)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Projectile Spawned"));
		}

		if (weap != NULL)
		{
			weap->fire();
		}

		//TO DO: MAKE THIS PLAY A WEAPON SPECIFIC SOUND
		//play the sound if it is not null
		if (fireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());
		}

		//TO DO: MAKE THIS PLAY A WEAPON SPECIFIC ANIMATION
		//play the animation if the animation & instance are not null
		if (fireAnim != NULL && animInstance != NULL)
		{
			animInstance->Montage_Play(fireAnim, 1.0f);
		}
	}
}

void AFPSPlayer::moveForward(float val)
{
	if (val != 0.0f) //if val is not nothing then
	{	//add movement input
		AddMovementInput(GetActorForwardVector(), val);
	}
}

void AFPSPlayer::moveRight(float val)
{
	if (val != 0.0f) //if val is not nothing then
	{	//add movement input
		AddMovementInput(GetActorRightVector(), val);
	}
}

void AFPSPlayer::turnAtRate(float rate)
{
	//Add yaw input (* by getDeltaSeconds for a smoother input)
	AddControllerYawInput(rate * turnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSPlayer::lookUpAtRate(float rate)
{
	//Add pitch input (* by getDeltaSeconds for a smoother input)
	AddControllerPitchInput(rate * lookUpRate * GetWorld()->GetDeltaSeconds());
}