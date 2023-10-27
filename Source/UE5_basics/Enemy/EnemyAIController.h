// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_BASICS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI: Team")
	float TeammateAdviseRadius = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI: behaviour tree")
	UBehaviorTree* BehaviourTree;
	
	/** Stop the AI job */
	UFUNCTION()
	void StopAI();

	/** Called to manage the player detection */
	UFUNCTION()
	void DetectPlayer();

protected:
	virtual void BeginPlay() override;

	/** This function handle all the senses.
	* Here are implemented all the function call to the ManageSense private functions
	*/
	UFUNCTION()
	void OnPerceptionUpdate_SenseManagement(const TArray<AActor*>& UpdateActors);

private:
	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;

	/** Function used to manege the sight sense*/
	void ManageSight();

	/** Function used to manege the hearing sense*/
	void ManageHearing();

	/** Notify teammate located in a radius equal to TeammateAdviseRadius */
	void NotifyTeammate();
};
