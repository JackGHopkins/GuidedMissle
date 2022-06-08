// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "GuidedMissleCharacter.h"
#include "GuidedMissleProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	bIsZoom = false;
}

UTP_WeaponComponent::~UTP_WeaponComponent()
{

}

void UTP_WeaponComponent::Fire()
{
	if(Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			FTransform SpawnLocAndRotation;
			SpawnLocAndRotation.SetLocation(SpawnLocation);
			SpawnLocAndRotation.SetRotation(SpawnRotation.Quaternion());

	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			AGuidedMissleProjectile* Projectile = World->SpawnActor<AGuidedMissleProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (TargetActor == nullptr) {
				//Projectile->FindTarget();
				Projectile->GetProjectileMovement()->bIsHomingProjectile = false;
				//Projectile->GetProjectileMovement()->HomingTargetComponent = TargetActor->FindComponentByClass<UMeshComponent>();
			}
			else {
				Projectile->GetProjectileMovement()->HomingTargetComponent = TargetActor->FindComponentByClass<UMeshComponent>();
				Projectile->GetProjectileMovement()->bIsHomingProjectile = true;
			}
				Projectile->FinishSpawning(SpawnLocAndRotation);
		}
		if(bIsZoom)
		{
			Character->GetFirstPersonCameraComponent()->SetFieldOfView(90.0f);
			bIsZoom = false;
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::Target() {
	if (Character != nullptr)
	{
		if (!bIsZoom) {
			Character->GetFirstPersonCameraComponent()->SetFieldOfView(20.0f);
			bIsZoom = true;
		}
		else
		{
			Character->GetFirstPersonCameraComponent()->SetFieldOfView(90.0f);
			bIsZoom = false;
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		// Unregister from the OnUseItem Event
		Character->OnUseItemPrimary.RemoveDynamic(this, &UTP_WeaponComponent::Fire);
		Character->OnUseItemSecondary.RemoveDynamic(this, &UTP_WeaponComponent::Target);
	}
}

void UTP_WeaponComponent::AttachWeapon(AGuidedMissleCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Register so that Fire is called every time the character tries to use the item being held
		Character->OnUseItemPrimary.AddDynamic(this, &UTP_WeaponComponent::Fire);
		Character->OnUseItemSecondary.AddDynamic(this, &UTP_WeaponComponent::Target);
	}
}

