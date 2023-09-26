#pragma once

#include "Throwable.h"

FThrowable::FThrowable() {
	this->WeaponMesh = NULL;
	this->MaxCapacity = 5;
	this->TimeForExplosion = 3.0f;
	this->Damage = 100.0f;
	this->DamageRadius = 100.0f;
	this->HitEFX = NULL;
	this->SoundEFX = NULL;
}
