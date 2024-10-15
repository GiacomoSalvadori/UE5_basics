// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTimedActor.h"

// Sets default values
AMyTimedActor::AMyTimedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTimedActor::BeginPlay()
{
	Super::BeginPlay();

	StartTimer();
}

// Called every frame
void AMyTimedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	//World->GetTimerManager().Tick();
}


void AMyTimedActor::StartTimer()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	if(!TimerManager.IsTimerActive(TimerHandler))
	{
		TimerManager.SetTimer(TimerHandler, this, &AMyTimedActor::TimerCallback, Duration, bUseInLoop);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Timer is set!");
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Timer is used!");
	}
}

void AMyTimedActor::TimerCallback()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Timer is over!");
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	World->GetTimerManager().ClearTimer(TimerHandler); // clean up the timer
	
	OnTimerRunsOut(); // this is the implementable event
}