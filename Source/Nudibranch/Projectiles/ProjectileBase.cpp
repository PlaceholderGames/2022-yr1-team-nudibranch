// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create collision sphere
	collSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	collSphere->InitSphereRadius(20.0f); //collision size

	//create mesh
	//projectileMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");

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
	
	//link the collision method with collision event
	collSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::onHit);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Hit event
void AProjectileBase::onHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hit)
{
	FDamageEvent damageEvent;

	if ((otherActor != NULL) && (otherActor != this) && (otherComp != NULL) && otherComp->IsSimulatingPhysics())
	{
		otherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	//apply damage to other actors
	if (otherActor != NULL)
	{
		otherActor->TakeDamage(100, damageEvent, this->GetInstigatorController(), this);
	}
}