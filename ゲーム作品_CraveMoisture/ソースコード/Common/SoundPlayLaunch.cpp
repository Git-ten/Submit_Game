#include <cassert>
#include<DxLib.h>
#include "SoundPlayLaunch.h"

// 音を鳴らしたくないときコメントアウト
#define DEBUG_SOUND (1)

SoundPlayLaunch::SoundPlayLaunch(void)
{
}

SoundPlayLaunch::~SoundPlayLaunch(void)
{
}

void SoundPlayLaunch::AddPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id, const int handleId)
{
	resourceSound_.emplace(id, handleId);
}

void SoundPlayLaunch::Play(const SoundResourceManager::SOUND_RESOURCE_ID& id, int type, bool isReplay)
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunchで空のリソースに参照しようとしました");

	if (IsPlaySound(id) && !isReplay)return;

	//DX_PLAYTYPE_NORMAL　:　ノーマル再生
	//DX_PLAYTYPE_BACK　　 : バックグラウンド再生
	//DX_PLAYTYPE_LOOP　　 : ループ再生

	if (type != DX_PLAYTYPE_NORMAL &&
		type != DX_PLAYTYPE_BACK   &&
		type != DX_PLAYTYPE_LOOP)
	{
		assert(!"無効な音再生タイプです");
	}

#ifdef DEBUG_SOUND
	PlaySoundMem(resourceSound_.at(id), type);
#endif
}

void SoundPlayLaunch::Stop(const SoundResourceManager::SOUND_RESOURCE_ID& id)
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunchで空のリソースに参照しようとしました");

#ifdef DEBUG_SOUND
	StopSoundMem(resourceSound_.at(id));
#endif
}

const bool SoundPlayLaunch::IsPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id)
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunchで空のリソースに参照しようとしました");

	return CheckSoundMem(resourceSound_.at(id)) > 0 ? true : false;
}

const int SoundPlayLaunch::GetHandleId(const SoundResourceManager::SOUND_RESOURCE_ID& id)const
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunchで空のリソースに参照しようとしました");

	return resourceSound_.at(id);
}
