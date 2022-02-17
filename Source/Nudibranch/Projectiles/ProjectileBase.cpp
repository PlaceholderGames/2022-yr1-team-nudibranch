// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create collision sphere
	collSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	collSphere->InitSphereRadius(20.0f); //collsion size

	//create mesh
	projectileMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");

	//set the root component to the collision sphere
	RootComponent = collSphere;

	//create projectile component
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	projectileMovement->UpdatedComponent = collSphere;
	projectileMovement->InitialSpeed = 6000.0f;
	projectileMovement->MaxSpeed = 6000.0f;
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->bShouldBounce = true;

	//despawn if nothing is hit after 3 seconds
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::onHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hit)
{
}