// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_basics/Enemy/Enemy.h"
#include "Subsystems/WorldSubsystem.h"
#include "MySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UE5_BASICS_API UMySubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AEnemy*> EnemiesInLevel;

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}
	
protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	/**
	* Rate-limited tick function.
	*/
	virtual void Tick(float DeltaTime) override;

private:

	void FindEnemies();
	
	float AccumulatedDeltaTime = 0.f;
	float TickRate = 0.5f;
	
};
