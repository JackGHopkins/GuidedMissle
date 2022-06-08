// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GuidedMissleCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItemPrimary);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItemSecondary);

UCLASS(config=Game)
class AGuidedMissleCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AGuidedMissleCharacter();

protected:
	virtual void BeginPlay();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItemPrimary OnUseItemPrimary;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItemSecondary OnUseItemSecondary;
protected:
	void OnPrimaryAction();
	void OnSecondaryAction();
	void MoveForward(float Val);
	void MoveRight(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};

