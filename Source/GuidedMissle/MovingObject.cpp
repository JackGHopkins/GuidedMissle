// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingObject.h"

// Sets default values
AMovingObject::AMovingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(SplineComponent);
}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();

	TriggerMovement();

}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
		MovementTimeline.TickTimeline(DeltaTime);
}

void AMovingObject::TriggerMovement() 
{
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, TEXT("ProcessMovementTimeline"));
	MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);

	FOnTimelineEvent OnTimelineFinishedFunction;
	OnTimelineFinishedFunction.BindUFunction(this, TEXT("OnEndMovementTimeline"));
	MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFunction);

	MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	MovementTimeline.Play();
	MovementTimeline.SetLooping(true);
}

void AMovingObject::ProcessMovementTimeline(float Value)
{
	const float SplineLength = SplineComponent->GetSplineLength();

	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	MeshComponent->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

