// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyPath.h"
#include "GameFramework/Character.h"
#include "UE5_basics/Other/HealthComponent.h"
#include "UE5_basics/Other/WeaponSlot.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateEnemy);
UCLASS()
class UE5_BASICS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AimOffset = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponSlot WeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	AEnemyPath* PathToPatrol;
		

private:
	

protected:
	
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void FireWithSphereSweep();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void CrouchMe();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void UncrouchMe();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void AimIn();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void AimOut();

	/** Broadcasted when character land on ground */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterLanding;

	/** Broadcasted when character jump */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterJumping;

	/** Broadcasted when character crouching */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterCrouch;

	/** Broadcasted when character stop crouching */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterUncrouch;

	/** Broadcasted when character start aiming */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterAim;

	/** Broadcsted when character stop aiming */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterStopAim;

	/** Broadcsted when character reloading the weapon */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterStartReload;

	/** Broadcsted when character trace line from weapon */
	UPROPERTY(BlueprintAssignable)
	FGameStateEnemy OnCharacterTraceLine;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

};
