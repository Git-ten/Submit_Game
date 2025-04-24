#include "CharactorBase.h"

CharactorBase::CharactorBase()
{
}

CharactorBase::~CharactorBase()
{
}

SceneManager::CHARA_ID CharactorBase::GetModelResource(void)
{
	return SceneManager::CHARA_ID::NONE;
}

float CharactorBase::GetAttackArea(void) const
{
	return 0.0f;
}

float CharactorBase::GetAvoidPower(void) const
{
	return 0.0f;
}

float CharactorBase::GetJumpPower(void) const
{
	return 0.0f;
}

float CharactorBase::GetDefencePower(void) const
{
	return 0.0f;
}

Color3D CharactorBase::GetMyColor(void) const
{
	return Color3D();
}

float CharactorBase::GetAttackHappen(ActorBase::STATE type) const
{
	return 0.0f;
}

float CharactorBase::GetSpeed() const
{
	return 0.0f;
}

void CharactorBase::SetActorCamera(std::weak_ptr<Camera>actCam)
{
	camera_ = actCam;
}

bool CharactorBase::IsUnleashSkill(void) const
{
	return false;
}

void CharactorBase::SkillCamera(void)
{
}

void CharactorBase::WeekFinishCamera(void)
{
}

void CharactorBase::StrongFinishCamera(void)
{
}

void CharactorBase::InitEffect(void)
{
}

void CharactorBase::InitEffectState(void)
{
}

void CharactorBase::InitSound(void)
{
}