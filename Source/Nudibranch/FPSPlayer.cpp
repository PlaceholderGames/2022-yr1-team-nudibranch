// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayer.h"

#include "FPSGameMode.h"
#include "Weapons/WeaponBase.h"

#include "GameFramework/CharacterMovementComponent.h"
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
	RootComponent = GetCapsuleComponent();

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

	//set offset for the gun
	gunOffset = FVector(100.0f, 0.0f, 10.0f);

	bIsSprinting = false;
	bIsSneaking = false;
	bIsCrouched = false;

	//set the health
	health = maxHealth;

}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//set the world to the current world
	world = GetWorld();

	//bind take damage event to apply damage method
	this->OnTakeAnyDamage.AddDynamic(this, &AFPSPlayer::applyDamage);

	//Check weapon & world exist
	if (WeaponClass && world) //if weapon class exists then add the weapon to the world and attach to the player
	{
		FActorSpawnParameters weapSpawnParams;
		weapSpawnParams.bNoFail = true;
		weapSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		weapSpawnParams.Owner = this;
		weapSpawnParams.Instigator = this;

		FTransform weapTransform;
		weapTransform.SetLocation(FVector::ZeroVector);
		weapTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		//create the weapon
		weap = world->SpawnActor<AWeaponBase>(WeaponClass, weapTransform, weapSpawnParams);

		if (weap) //if weapon is created
		{
			//attach the weapon to the hands
			weap->AttachToComponent(handsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
		}
	}

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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlayer::startFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSPlayer::stopFire);

	//bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSPlayer::reload);

	//bind crouch event
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSPlayer::crouch);

	//bind sneak event
	PlayerInputComponent->BindAction("Sneak", IE_Pressed, this, &AFPSPlayer::startSneak);
	PlayerInputComponent->BindAction("Sneak", IE_Released, this, &AFPSPlayer::stopSneak);

	//bind sprint event
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSPlayer::startSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSPlayer::stopSprint);

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


void AFPSPlayer::applyDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* instigator, AActor* damageCauser)
{
	health -= damage;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Health: " + FString::SanitizeFloat(health)));
}

/*

		WEAPON FUNCTIONS

*/

void AFPSPlayer::startFire()
{
	if (weap != nullptr)
	{
		weap->startFire();
	}
}

void AFPSPlayer::stopFire()
{
	if (weap != nullptr)
	{
		weap->stopFire();
	}
}

void AFPSPlayer::reload()
{
	if (weap != nullptr) 
	{
		weap->reload(); 
	}
}

//Used by the HUD to display the current ammo
FString AFPSPlayer::getAmmoDisplay()
{
	FString str = "Ammo: " + FString::SanitizeFloat(weap->getClipAmmo()) + " / " + FString::SanitizeFloat(weap->getResAmmo()); //should change this to a current weapon ptr
	return str;
}


/*

		MOVEMENT FUNCTIONS

*/

void AFPSPlayer::crouch()
{
	bIsCrouched = !bIsCrouched; //toggle crouch

	if (bIsCrouched)
	{
		GetCharacterMovement()->bWantsToCrouch = true;
		GetCharacterMovement()->Crouch(true);
		//GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Crouched"));
	}
	else
	{
		GetCharacterMovement()->bWantsToCrouch = false;
		GetCharacterMovement()->UnCrouch(true);
		//GetCapsuleComponent()->SetCapsuleHalfHeight(95.0f);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Uncrouched"));
	}
}

void AFPSPlayer::startSneak()
{
	bIsSneaking = true;
}

void AFPSPlayer::stopSneak()
{
	bIsSneaking = false;
}

void AFPSPlayer::startSprint()
{
	bIsSprinting = true;
}

void AFPSPlayer::stopSprint()
{
	bIsSprinting = false;
}

void AFPSPlayer::moveForward(float val)
{
	if (val != 0.0f) //if val is not nothing then
	{	//add movement input

		if (bIsSprinting && !bIsCrouched)
		{
			val *= sprintMultiplier;
		}
		else if (bIsSneaking)
		{
			val *= sneakMultiplier;
		}
		else
		{
			val *= walkMultiplier;
		}

		AddMovementInput(GetActorForwardVector(), val);
	}
}

void AFPSPlayer::moveRight(float val)
{
	if (val != 0.0f) //if val is not nothing then
	{	//add movement input

		if (bIsSneaking)
		{
			val *= sneakMultiplier;
		}
		else
		{
			val *= walkMultiplier;
		}

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