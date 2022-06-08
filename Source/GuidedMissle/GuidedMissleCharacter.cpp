// Copyright Epic Games, Inc. All Rights Reserved.

#include "GuidedMissleCharacter.h"
#include "GuidedMissleProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// AGuidedMissleCharacter

AGuidedMissleCharacter::AGuidedMissleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

}

void AGuidedMissleCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void AGuidedMissleCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AGuidedMissleCharacter::OnPrimaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &AGuidedMissleCharacter::OnSecondaryAction);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGuidedMissleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGuidedMissleCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGuidedMissleCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGuidedMissleCharacter::LookUpAtRate);
}

void AGuidedMissleCharacter::OnPrimaryAction()
{
	OnUseItemPrimary.Broadcast();
}

void AGuidedMissleCharacter::OnSecondaryAction() 
{
	OnUseItemSecondary.Broadcast();
}

void AGuidedMissleCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorForwardVector(), Value);
}

void AGuidedMissleCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
		AddMovementInput(GetActorRightVector(), Value);
}

void AGuidedMissleCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AGuidedMissleCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}
