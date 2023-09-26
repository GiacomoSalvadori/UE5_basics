// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPath.generated.h"

UCLASS()
class UE5_BASICS_API AEnemyPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Points", Meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;

private:
	/** Path sense orientation, am I backtracking the path? */
	int PathSense;
	/** An integer representing the path index */
	int PathIndex;

protected:

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Set the index to next node */
	UFUNCTION(BlueprintCallable, Category = "AI Path")
	void GoNextNode();

	/** Retrieve actual point */
	UFUNCTION(BlueprintCallable, Category = "AI Path")
	FVector ActualPoint();

};
