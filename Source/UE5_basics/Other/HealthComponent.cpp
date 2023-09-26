// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}


void UHealthComponent::BeginPlay() {
	Super::BeginPlay();

	HealthDefaultValue = Health;
	HealthMaxValue = Health;
	ElapsedTime = 0.0f;
	TimeSinceLastDamage = 0.0f;
	bIsDamaged = false;
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AutoRecoveryHealth(DeltaTime);
	CheckDamageTime(DeltaTime);
}


void UHealthComponent::GetDamage(float Amount) {
	Health = FMath::Clamp(Health - Amount, 0.0f, HealthMaxValue);
	bIsDamaged = true;
	TimeSinceLastDamage = GetWorld()->GetTimeSeconds();
	OnGetDamage.Broadcast();
	if (Health <= 0) {
		OnHealtToZero.Broadcast();
	}
}

void UHealthComponent::IncrementMaxHealth(float Amount) {
	HealthMaxValue += Amount;
}

void UHealthComponent::Healing(float Amount) {
	Health = FMath::Clamp(Health + Amount, 0.0f, HealthMaxValue);
}

void UHealthComponent::CheckDamageTime(float DeltaTime) {
	if (bIsDamaged) {
		float ActualTime = GetWorld()->GetTimeSeconds();
		if ((ActualTime - TimeSinceLastDamage) >= NoDamageTimeForRecovery) {
			bIsDamaged = false;
		}
	}
}

void UHealthComponent::AutoRecoveryHealth(float DeltaTime) {
	if (bAutoRecovery && !bIsDamaged) {
		ElapsedTime += DeltaTime;
		
		if (ElapsedTime >= HealthRecoveryTime) {
			ElapsedTime = 0.0f;
			Healing(RecoveryQuantity);
			OnHealthRecovery.Broadcast();
		}		
	}
}

float UHealthComponent::HealthPercentage() {
	return HealthMaxValue == 0.0 ? 1.0f : (Health/HealthMaxValue);
}
