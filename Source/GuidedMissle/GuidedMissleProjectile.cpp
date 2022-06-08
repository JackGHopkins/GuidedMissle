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
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGuidedMissleProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

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

