// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTimedActor.generated.h"

// How to declare an abstract class and virtual method
// https://www.youtube.com/watch?v=UvxyQ7hEgbg&ab_channel=PobatoTutorials
// timer reference
// https://www.tomlooman.com/unreal-engine-cpp-guide/
// delegate form
// https://benui.ca/unreal/delegates-advanced/
UCLASS()
class UE5_BASICS_API AMyTimedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTimedActor();

protected:

	// Actor's time handler.
	FTimerHandle TimerHandler;

	virtual void BeginPlay() override;

public:

	// The timer should loop?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseInLoop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 3.0f;

	UFUNCTION(BlueprintCallable)
	void StartTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTimerRunsOut();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Called when the timer runs out 
	void TimerCallback();
};
