// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UexperimentGameModeBase.h"

#define BULLET_RADIUS 20.0f

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphere;
	sphere->InitSphereRadius(BULLET_RADIUS);
	sphere->SetCollisionProfileName(TEXT("Bullet"));

	UStaticMeshComponent* visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	visual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> visualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (visualAsset.Succeeded())
	{
		visual->SetStaticMesh(visualAsset.Object);
		visual->SetWorldScale3D(FVector(BULLET_RADIUS / 50.0f));
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);

	auto gameMode = (AUexperimentGameModeBase*)GetWorld()->GetAuthGameMode();
	if (FMath::Abs(NewLocation.X) < gameMode->WorldBounds && FMath::Abs(NewLocation.Y) < gameMode->WorldBounds)
	{
		SetActorLocation(NewLocation);
	}
	else
	{
		GetWorld()->DestroyActor(this);
	}
}

void ABullet::Init(const FVector& direction)
{
	auto gameMode = (AUexperimentGameModeBase*)GetWorld()->GetAuthGameMode();

	Velocity = direction;
	Velocity.Normalize();
	Velocity *= gameMode->BulletVelecoty;
}

