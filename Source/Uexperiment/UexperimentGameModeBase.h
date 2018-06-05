// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UexperimentGameModeBase.generated.h"

class AMyPawn;

UCLASS()
class UEXPERIMENT_API AUexperimentGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	AUexperimentGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AMyPawn * GetPlayer() const;

	UPROPERTY(EditAnywhere)
	float WorldBounds;

	UPROPERTY(EditAnywhere)
	float BulletVelecoty;

	UPROPERTY(Category=Enemy, EditAnywhere)
	float EnemySpawnTime;

	UPROPERTY(Category=Enemy, EditAnywhere)
	float InitialEnemySpawnTime;

	UPROPERTY(Category=Enemy, EditAnywhere)
	float EnemySpeed;

	UPROPERTY(Category=Enemy, EditAnywhere)
	float EnemyMinSpawnDistance;

private:
	void SpawnEnemy(int requiredEnemies);
	int GetCurrentNumEnemies();

	float _NextEnemySpawnTime;
	int _NumEnemies;
};
