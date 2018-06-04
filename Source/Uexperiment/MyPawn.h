// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class USpringArmComponent;

UCLASS()
class UEXPERIMENT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* OurVisibleComponent;

	UPROPERTY(Category="Tweaks", EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(Category="Tweaks", EditAnywhere)
	float Acceleration;

	UPROPERTY(Category="Tweaks", EditAnywhere)
	float Drag;

	UPROPERTY(Category="Tweaks", EditAnywhere)
	float ShotCoolDown;

	//Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void Shoot_XAxis(float AxisValue);
	void Shoot_YAxis(float AxisValue);
	void CameraZoom(float AxisValue);

	//Input variables
	FVector CurrentAcceleration;
	FVector CurrentVelocity;
	FVector ShotDirection;
	float ShotTime;
};
