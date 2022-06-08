// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_WeaponComponent.generated.h"

class AGuidedMissleCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDEDMISSLE_API UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AGuidedMissleProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bIsZoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	AGuidedMissleCharacter* Character;

	UTP_WeaponComponent();

	~UTP_WeaponComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AGuidedMissleCharacter* TargetCharacter);
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Target();

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

};
