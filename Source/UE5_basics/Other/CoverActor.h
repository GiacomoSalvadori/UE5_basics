// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CoverActor.generated.h"

UCLASS()
class UE5_BASICS_API ACoverActor : public AActor
{
	GENERATED_BODY()
	
private:
	/** Returns true if the socket is close to the player */
	float DistanceFromPlayer(FName SocketName);

	/** Determines the movement direction and the facing direction of the player */
	void DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	/** Returns the name of the nearby socket */
	FName GetNearbySocket();

	/** Given a socket return the opposite one */
	FName GetOppositeSocket(FName Socket);
	
protected:

	/** The box component that informs the player if he's able to take cover or not */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

public:
	// Sets default values for this actor's properties
	ACoverActor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
	FVector BoxCompOffset = FVector(50.0f, 50.0f, 0.0f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* SM;

	/** Retrieves the movement direction and the facing rotation of the player */
	void RetrieveMovementDirectionAndFacingRotation(FVector& MovementDirection, FRotator& FacingRotation);

	/** Get the cover position better hidden from player*/
	UFUNCTION(BlueprintCallable, Category = "Cover")
	FVector GetPositionHiddenFromPlayer();

};
