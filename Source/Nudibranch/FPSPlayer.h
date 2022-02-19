// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlayer.generated.h"

UCLASS()
class NUDIBRANCH_API AFPSPlayer : public ACharacter
{
	GENERATED_BODY()

// ****** DEFAULT UE STUFF ******

public:
	// Sets default values for this character's properties
	AFPSPlayer();
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
	
	class UWorld* world;
	class UAnimInstance* animInstance;

	FRotator spawnRotation;
	FVector spawnLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* handsMesh;

	//single weapon code : remove after implementing the weapon base system
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		//class USkeletalMeshComponent* gunMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* muzzleLocation;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* fpsCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float turnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float lookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		FVector gunOffset;

	//single weapon code : remove after implementing the weapon base system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* fireSound;

	//single weapon code : remove after implementing the weapon base system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* fireAnim;

protected:

	bool bIsSprinting = false;
	bool bIsSneaking = false;
	float sprintMultiplier = 1.0f;
	float walkMultiplier = 0.70f;
	float sneakMultiplier = 0.25f;

	void startFire();
	void stopFire();
	void reload();

	void crouch();
	void startSneak();
	void stopSneak();
	void startSprint();
	void stopSprint();

	void moveForward(float val);
	void moveRight(float val);
	void turnAtRate(float rate);
	void lookUpAtRate(float rate);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AProjectileBase> ProjectileClass;

	class AWeaponBase* weap;
	class AProjectileBase* projectile;

};
