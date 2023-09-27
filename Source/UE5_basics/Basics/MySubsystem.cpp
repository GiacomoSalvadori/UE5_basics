// Fill out your copyright notice in the Description page of Project Settings.


#include "MySubsystem.h"
#include "Kismet/GameplayStatics.h"

void UMySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, "Init subsystem!");
	FindEnemies();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, "Init subsystem "+FString::SanitizeFloat(EnemiesInLevel.Num()));
}

void UMySubsystem::Deinitialize()
{
	
}

void UMySubsystem::Tick(const float DeltaTime)
{
	if((AccumulatedDeltaTime += DeltaTime) < TickRate)
	{
		return;
	}
	AccumulatedDeltaTime = 0.f;

	FindEnemies(); // not the best way in order to achieve this, but it's okay for the sake of didactics
}

void UMySubsystem::FindEnemies()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);
	EnemiesInLevel.Empty();
	
	for(const auto& Enemy : FoundActors)
	{
		if(IsValid(Enemy))
		{
			AEnemy* EnemyCast = dynamic_cast<AEnemy*>(Enemy);
			if(IsValid(EnemyCast))
			{
				EnemiesInLevel.Add(EnemyCast);
			}
		}
	}
}