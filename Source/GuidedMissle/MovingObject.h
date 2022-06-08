// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingObject.generated.h"

UCLASS()
class GUIDEDMISSLE_API AMovingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	UCurveFloat* MovementCurve;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TriggerMovement();

	UFUNCTION()
	void ProcessMovementTimeline(float Value);

protected:
	virtual void BeginPlay() override;
	
private:
	FTimeline MovementTimeline;
};
