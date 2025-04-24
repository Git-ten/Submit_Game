#include <DxLib.h>
#include<EffekseerForDxLib.h>
#include "EffectPlayLaunch.h"

#pragma region 定数宣言

// 初期化子
const float INIT_ZERO = 0.0f;

// 例外子
const int EXCEPTION_VALUE = -1;
#pragma endregion

EffectPlayLaunch::EffectPlayLaunch(void)
{
}

EffectPlayLaunch::~EffectPlayLaunch(void)
{
}

void EffectPlayLaunch::AddPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const int handleId)
{
	effect_.resourceEffects.emplace(id, handleId);
	effect_.playEffects.emplace(id, EXCEPTION_VALUE);
	effect_.followPos.emplace(id, nullptr);
	effect_.followRot.emplace(id, nullptr);
	effect_.pos.emplace(id, Position3DF());
	effect_.speed.emplace(id, INIT_ZERO);
	effect_.scl.emplace(id, Size3DF());
}

void EffectPlayLaunch::Play(const EffectResourceManager::EFFECT_RESOURCE_ID& id,
	const DataEffect& data)
{
	CheckPlayEffect(id);

	if (IsPlayEffect(id))return;

	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);
	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		effect_.playEffects.at(id) = PlayEffekseer3DEffect(effect_.resourceEffects.at(id));
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		effect_.playEffects.at(id) = PlayEffekseer2DEffect(effect_.resourceEffects.at(id));
		break;
	default:
		break;
	}

	SetScl(id, data.scl);
	SetSpeed(id, data.speed);

	// ここで追従座標は初期化
	effect_.followPos.at(id) = nullptr;
	effect_.followRot.at(id) = nullptr;
}

void EffectPlayLaunch::FollowPlay(const EffectResourceManager::EFFECT_RESOURCE_ID& id,
	const DataEffect& data, Position3DF& pos, Rot3DF* rot)
{
	Play(id, data);
	effect_.followPos.at(id) = &pos;
	effect_.followRot.at(id) = rot;
}

void EffectPlayLaunch::UpdateFollow(void)
{
	for (const auto& effect : effect_.followPos)
	{
		if (effect.second != nullptr)
			SetPos(effect.first, *effect.second);
	}

	for (const auto& effect : effect_.followRot)
	{
		if (effect.second != nullptr)
			SetRot(effect.first, *effect.second);
	}

}

void EffectPlayLaunch::Stop(const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	if (!IsPlayEffect(id))return;
	CheckPlayEffect(id);

	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);

	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		StopEffekseer3DEffect(effect_.playEffects.at(id));
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		StopEffekseer2DEffect(effect_.playEffects.at(id));
		break;
	default:
		break;
	}
}

void EffectPlayLaunch::AllStop(void)
{
	for (const auto& effects : effect_.playEffects)
	{
		Stop(effects.first);
	}
}

const bool EffectPlayLaunch::IsPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);
	CheckPlayEffect(id);

	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		return IsEffekseer3DEffectPlaying(effect_.playEffects.at(id)) == 0 ? true : false;
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		return IsEffekseer2DEffectPlaying(effect_.playEffects.at(id)) == 0 ? true : false;
		break;
	default:
		return false;
		break;
	}
}

void EffectPlayLaunch::MoveEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id,
	const Dir3DF& dir, const float& distance)
{
	VECTOR vec = VScale(dir, distance);
	Position3DF pos = effect_.pos.at(id);
	pos = VAdd(pos, vec);

	SetPos(id, pos);
}

const int EffectPlayLaunch::GetPlayId(const EffectResourceManager::EFFECT_RESOURCE_ID& id) const
{
	return effect_.playEffects.at(id);
}

const Position3DF& EffectPlayLaunch::GetPos(const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	CheckPlayEffect(id);
	return effect_.pos.at(id);
}

const float EffectPlayLaunch::GetSpeed(const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	CheckPlayEffect(id);
	return effect_.speed.at(id);
}

const Size3DF& EffectPlayLaunch::GetScl(const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	CheckPlayEffect(id);
	return effect_.scl.at(id);
}

void EffectPlayLaunch::SetPos(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Position3DF& pos)
{
	if (!IsPlayEffect(id))return;
	CheckPlayEffect(id);

	effect_.pos.at(id) = pos;

	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);
	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		SetPosPlayingEffekseer3DEffect(effect_.playEffects.at(id), pos.x, pos.y, pos.z);
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		SetPosPlayingEffekseer2DEffect(effect_.playEffects.at(id), pos.x, pos.y, pos.z);
		break;
	default:
		break;
	}
}

void EffectPlayLaunch::SetRot(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Rot3DF& rot)
{
	if (!IsPlayEffect(id))return;
	CheckPlayEffect(id);

	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);
	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		SetRotationPlayingEffekseer3DEffect(effect_.playEffects.at(id), rot.x, rot.y, rot.z);
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		SetRotationPlayingEffekseer2DEffect(effect_.playEffects.at(id), rot.x, rot.y, rot.z);
		break;
	default:
		break;
	}
}

void EffectPlayLaunch::SetScl(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Size3DF& scl)
{
	if (!IsPlayEffect(id))return;
	CheckPlayEffect(id);

	effect_.scl.at(id) = scl;

	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);
	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		SetScalePlayingEffekseer3DEffect(effect_.playEffects.at(id), scl.x, scl.y, scl.z);
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		SetScalePlayingEffekseer2DEffect(effect_.playEffects.at(id), scl.x, scl.y, scl.z);
		break;
	default:
		break;
	}
}

void EffectPlayLaunch::SetSpeed(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const float speed)
{
	if (!IsPlayEffect(id))return;
	CheckPlayEffect(id);

	effect_.speed.at(id) = speed;

	EffectResource::EFFECT_TYPE type = EffectResourceManager::GetInstance().GetEffectType(id);
	switch (type)
	{
	case EffectResource::EFFECT_TYPE::EFFEKSEER_3D:
		SetSpeedPlayingEffekseer3DEffect(effect_.playEffects.at(id), speed);
		break;
	case EffectResource::EFFECT_TYPE::EFFEKSEER_2D:
		SetSpeedPlayingEffekseer2DEffect(effect_.playEffects.at(id), speed);
		break;
	default:
		break;
	}
}

void EffectPlayLaunch::CheckPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	const auto& resIte = effect_.resourceEffects.find(id);
	const auto& playIte = effect_.playEffects.find(id);
	const auto& followPosIte = effect_.followPos.find(id);
	const auto& posIte = effect_.pos.find(id);
	const auto& speedIte = effect_.speed.find(id);

	assert(resIte != effect_.resourceEffects.end() || !"エフェクトリソースが登録できていません");
	assert(playIte != effect_.playEffects.end() || !"エフェクトプレイIDが存在していません");
	assert(posIte != effect_.pos.end() || !"エフェクトの座標が存在していません");
	assert(speedIte != effect_.speed.end() || !"エフェクトの速度が存在していません");
	assert(followPosIte != effect_.followPos.end() || !"エフェクトの追従座標が存在していません");
}