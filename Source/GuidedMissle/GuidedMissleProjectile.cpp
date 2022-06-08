// Copyright Epic Games, Inc. All Rights Reserved.

#include "GuidedMissleProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AGuidedMissleProjectile::AGuidedMissleProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGuidedMissleProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"), false);
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->HomingAccelerationMagnitude = 10000.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	FTickFunction = new FActorComponentTickFunction();
	
	InitialLifeSpan = 10.0f;
	if (ProjectileMovement->HomingTargetComponent == nullptr)
		ProjectileMovement->bIsHomingProjectile = false;
}

AGuidedMissleProjectile::~AGuidedMissleProjectile() {
	delete FTickFunction;
}

void AGuidedMissleProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	}

	if (ExplosionSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation());
	}

	if (ExplosionAnimation != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionAnimation, this->GetActorLocation());
	}

	Destroy();
}

void AGuidedMissleProjectile::FindTarget() 
{
	class UWorld* const World = GetWorld();

	if (World) {
		for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			FName TargetTag = FName(TEXT("Target"));
			AActor* Act = *ActorItr;
			if (ActorItr->ActorHasTag(TargetTag)) {
				ProjectileMovement->HomingTargetComponent = Act->FindComponentByClass<UMeshComponent>();
				ProjectileMovement->bIsHomingProjectile = true;
			}
		}
	}
}
