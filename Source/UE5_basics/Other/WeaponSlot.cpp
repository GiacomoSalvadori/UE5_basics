
#include "WeaponSlot.h"


FWeaponSlot::FWeaponSlot() {
	this->WeaponMesh = NULL;
	this->MagCapacity = 10;
	this->IsAutomatic = true;
	this->Rate = 0.2f;
	this->Damage = 20.0f;
	this->Range = 10000.0f;
	this->HitRadius = 50.0f;
	this->Offset = 55.0f;
	this->HitEFX = NULL;
	this->SoundEFX = NULL;
}
