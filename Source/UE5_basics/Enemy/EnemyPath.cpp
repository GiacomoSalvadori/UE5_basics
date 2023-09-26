// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPath.h"

AEnemyPath::AEnemyPath() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyPath::BeginPlay() {
	Super::BeginPlay();
	
	PathSense = 1;
	PathIndex = 0;
}

void AEnemyPath::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AEnemyPath::OnConstruction(const FTransform & Transform) {
	if (PathPoints.Num() == 0) {
		FVector FirstLocation = FVector::ZeroVector;
		PathPoints.Add(FirstLocation);
		FVector SecondLocation = FirstLocation + FVector(100.0f, 0.0f, 0.0f);
		PathPoints.Add(SecondLocation);
	}
}

void AEnemyPath::GoNextNode() {
	PathIndex += PathSense;
	if (PathIndex == PathPoints.Num()-1 || PathIndex == 0) {
		PathSense *= -1;
	}
}

FVector AEnemyPath::ActualPoint() {
	FVector Pos = GetActorTransform().TransformPosition(PathPoints[PathIndex]);
	return Pos;
}


