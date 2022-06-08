// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuidedMissleProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USoundBase;
class USphereComponent;

UCLASS(config=Game)
class AGuidedMissleProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	FActorComponentTickFunction* FTickFunction;

public:
	AGuidedMissleProjectile();
	~AGuidedMissleProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UNiagaraSystem* ExplosionAnimation;
};

