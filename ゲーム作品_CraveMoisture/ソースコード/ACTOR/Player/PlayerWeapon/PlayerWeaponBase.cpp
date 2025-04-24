#include "PlayerWeaponBase.h"

PlayerWeaponBase::PlayerWeaponBase(int& modelId):
	modelId_(modelId)
{
}

PlayerWeaponBase::~PlayerWeaponBase(void)
{
}

void PlayerWeaponBase::SetUpAttackPower(float& power)
{
	myActorCollision_.damagePower = &power;
}

void PlayerWeaponBase::OnCollAttack(void)
{
	myActorCollision_.isCollAttack_ = true;
}

void PlayerWeaponBase::OffCollAttack(void)
{
	myActorCollision_.isCollAttack_ = false;
}

bool PlayerWeaponBase::IsCollAttack(void)
{
	return myActorCollision_.isCollAttack_;
}

void PlayerWeaponBase::SyncroPlayerWeapon(const int modelId, const std::string& frameName)
{
	if (actorTransform_ == nullptr)return;
	actorTransform_->Update(modelId, frameName);
}

void PlayerWeaponBase::InitActorTransform(void)
{
}

void PlayerWeaponBase::InitSpecial(void)
{
}

void PlayerWeaponBase::UpdateSpecial(void)
{
}

void PlayerWeaponBase::DrawSpecial(void)
{
}