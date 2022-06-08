#include "TP_WeaponComponent.h"
#include "GuidedMissleCharacter.h"
#include "GuidedMissleProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Kismet/GameplayStatics.h"

UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	bIsAiming = false;
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

	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			FTransform SpawnLocAndRotation;
			SpawnLocAndRotation.SetLocation(SpawnLocation);
			SpawnLocAndRotation.SetRotation(SpawnRotation.Quaternion());

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			AGuidedMissleProjectile* Projectile = World->SpawnActor<AGuidedMissleProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (TargetActor == nullptr) {
				Projectile->GetProjectileMovement()->bIsHomingProjectile = false;
			}
			else {
				Projectile->GetProjectileMovement()->HomingTargetComponent = TargetActor->FindComponentByClass<UMeshComponent>();
				Projectile->GetProjectileMovement()->bIsHomingProjectile = true;
			}
				Projectile->FinishSpawning(SpawnLocAndRotation);
		}
		if(bIsAiming)
		{
			Character->GetFirstPersonCameraComponent()->SetFieldOfView(90.0f);
			bIsAiming = false;
		}
	}
	
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	if (FireAnimation != nullptr)
	{
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
		if (!bIsAiming) {
			Character->GetFirstPersonCameraComponent()->SetFieldOfView(20.0f);
			bIsAiming = true;
		}
		else
		{
			Character->GetFirstPersonCameraComponent()->SetFieldOfView(90.0f);
			bIsAiming = false;
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		Character->OnUseItemPrimary.RemoveDynamic(this, &UTP_WeaponComponent::Fire);
		Character->OnUseItemSecondary.RemoveDynamic(this, &UTP_WeaponComponent::Target);
	}
}

void UTP_WeaponComponent::AttachWeapon(AGuidedMissleCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		Character->OnUseItemPrimary.AddDynamic(this, &UTP_WeaponComponent::Fire);
		Character->OnUseItemSecondary.AddDynamic(this, &UTP_WeaponComponent::Target);
	}
}

