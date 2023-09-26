
#pragma once

#include "Engine.h"
#include "Sound/SoundBase.h"
#include "WeaponSlot.generated.h"


USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh *WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MagCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* HitEFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundEFX;

	FWeaponSlot();
};
