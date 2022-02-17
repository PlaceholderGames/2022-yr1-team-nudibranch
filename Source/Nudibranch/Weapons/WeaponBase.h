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

	void fire();
	
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* weapMesh;

	UPROPERTY(EditAnywhere)
		class UAnimationAsset* fireAnim;

};
