// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UexperimentGameModeBase.h"
#include "MyPawn.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* root = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	root->bGenerateOverlapEvents = true;
	RootComponent = root;
	root->SetCollisionProfileName(TEXT("Enemy"));

	UStaticMeshComponent* visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	visual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> visualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (visualAsset.Succeeded())
	{
		visual->SetStaticMesh(visualAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> materialAssert(TEXT("/Game/EnemyMat.EnemyMat"));
	if (materialAssert.Succeeded())
	{
		visual->OverrideMaterials.Add(materialAssert.Object);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto gameMode = (AUexperimentGameModeBase*)GetWorld()->GetAuthGameMode();
	auto player = gameMode->GetPlayer();
	auto vectorToPlayer = player->GetActorLocation() - GetActorLocation();
	vectorToPlayer.Normalize();
	vectorToPlayer *= (DeltaTime * gameMode->EnemySpeed);

	SetActorLocation(GetActorLocation() + vectorToPlayer);
}

