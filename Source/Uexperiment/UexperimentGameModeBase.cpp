// Fill out your copyright notice in the Description page of Project Settings.

#include "UexperimentGameModeBase.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Enemy.h"
#include "MyPawn.h"

AUexperimentGameModeBase::AUexperimentGameModeBase() :
	WorldBounds(2000.0f),
	BulletVelecoty(2500.0f),
	EnemySpawnTime(3.0f),
	InitialEnemySpawnTime(5.0f),
	EnemySpeedMin(300.0f),
	EnemySpeedMax(600.0f),
	EnemyMinSpawnDistance(400.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUexperimentGameModeBase::BeginPlay()
{
	_NextEnemySpawnTime = InitialEnemySpawnTime;
	_NumEnemies = 0;
	EnemyMinSpawnDistance *= EnemyMinSpawnDistance;
}

void AUexperimentGameModeBase::Tick(float DeltaTime)
{
	_NextEnemySpawnTime -= DeltaTime;
	if (_NextEnemySpawnTime <= 0.0f)
	{
		_NumEnemies++;
		auto requiredEnemies = _NumEnemies - GetCurrentNumEnemies();
		SpawnEnemy(requiredEnemies);
		_NextEnemySpawnTime = EnemySpawnTime;
	}
}

AMyPawn* AUexperimentGameModeBase::GetPlayer() const
{
	auto it = TActorIterator<AMyPawn>(GetWorld());
	return *it;
}

void AUexperimentGameModeBase::SpawnEnemy(int requiredEnemies)
{
	auto player = GetPlayer();
	auto playerPos = player->GetActorLocation();

	while (requiredEnemies --> 0)
	{
		FVector spawnPos;

		do
		{
			auto x = FMath::RandRange(-WorldBounds, WorldBounds);
			auto y = FMath::RandRange(-WorldBounds, WorldBounds);
			spawnPos = FVector(x, y, 0.0f);
		} while (FVector::DistSquared(playerPos, spawnPos) < EnemyMinSpawnDistance);

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEnemy>(spawnPos, FRotator::ZeroRotator, params);
	}
}
int AUexperimentGameModeBase::GetCurrentNumEnemies()
{
	int count = 0;
	for (TActorIterator<AEnemy> it(GetWorld()); it; ++it)
		count++;

	return count;
}