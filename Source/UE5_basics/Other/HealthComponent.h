// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealtDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_BASICS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

private:
	float HealthDefaultValue;
	float HealthMaxValue;
	float ElapsedTime;
	float TimeSinceLastDamage;
	bool bIsDamaged;

	void AutoRecoveryHealth(float DeltaTime);

	void CheckDamageTime(float DeltaTime);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Brodcasted when the actor get damage */
	UPROPERTY(BlueprintAssignable)
	FHealtDelegate OnGetDamage;

	/** Broadcasted when Health values is <= 0 */
	UPROPERTY(BlueprintAssignable)
	FHealtDelegate OnHealtToZero;

	/** Broadcasted every time Health is recovered */
	UPROPERTY(BlueprintAssignable)
	FHealtDelegate OnHealthRecovery;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healt: variables")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healt: variables")
	bool bAutoRecovery = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healt: variables")
	float RecoveryQuantity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healt: variables")
	float HealthRecoveryTime = 0.2f;

	/** Time to wait for recovery to start after receiving damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healt: variables")
	float NoDamageTimeForRecovery = 2.5f;

	void GetDamage(float Amount);

	void IncrementMaxHealth(float Amount);

	void Healing(float Amount);
	
	/** Retrieve the health percentage */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float HealthPercentage();

		
};
