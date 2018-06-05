// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bullet.h"
#include "UexperimentGameModeBase.h"

#define CAMERA_DISTANCE 800.0f
#define CAMERA_ZOOMOUT_DISTANCE 1600.0f
#define SHOT_COOLDOWN_TIME 0.5f

// Sets default values
AMyPawn::AMyPawn() :
	Acceleration(150.0f),
	Drag(0.9f),
	CurrentAcceleration(0.0f),
	CurrentVelocity(0.0f),
	ShotDirection(0.0f),
	ShotCoolDown(SHOT_COOLDOWN_TIME),
	ShotTime(0.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	auto sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphere;

	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurVisibleComponent->SetupAttachment(sphere);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->RelativeRotation = FRotator(-45.0f, 0.0f, 0.0f);
	SpringArm->TargetArmLength = CAMERA_DISTANCE;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bDoCollisionTest = false;

	auto camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShotTime <= 0.0f && (0.5f*0.5f) <= ShotDirection.SizeSquared())
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto bullet = GetWorld()->SpawnActor<ABullet>(GetActorLocation(), FRotator::ZeroRotator, params);
		if (bullet) // A bullet can trigger a hit and removal instantly, giving us a null actor
			bullet->Init(ShotDirection);
		ShotTime = ShotCoolDown;
	}
	ShotTime -= DeltaTime;

	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		CurrentVelocity += CurrentAcceleration;
		CurrentVelocity *= Drag;

		if (!CurrentVelocity.IsNearlyZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			auto gameMode = (AUexperimentGameModeBase*)GetWorld()->GetAuthGameMode();
			auto worldBounds = gameMode->WorldBounds;
			NewLocation.X = FMath::Clamp(NewLocation.X, -worldBounds, worldBounds);
			NewLocation.Y = FMath::Clamp(NewLocation.Y, -worldBounds, worldBounds);

			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	PlayerInputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);
	PlayerInputComponent->BindAxis("ShootX", this, &AMyPawn::Shoot_XAxis);
	PlayerInputComponent->BindAxis("ShootY", this, &AMyPawn::Shoot_YAxis);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AMyPawn::CameraZoom);
}

void AMyPawn::Move_XAxis(float AxisValue)
{
	CurrentAcceleration.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * Acceleration;
}

void AMyPawn::Move_YAxis(float AxisValue)
{
	CurrentAcceleration.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * Acceleration;
}

void AMyPawn::Shoot_XAxis(float AxisValue)
{
	ShotDirection.X = AxisValue;
}

void AMyPawn::Shoot_YAxis(float AxisValue)
{
	ShotDirection.Y = AxisValue;
}
void AMyPawn::CameraZoom(float AxisValue)
{
	auto zoominess = (CAMERA_ZOOMOUT_DISTANCE - CAMERA_DISTANCE) * AxisValue;
	SpringArm->TargetArmLength = CAMERA_DISTANCE + zoominess;
}
