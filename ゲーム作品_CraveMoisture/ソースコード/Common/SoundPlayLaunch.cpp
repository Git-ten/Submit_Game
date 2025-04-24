#include <cassert>
#include<DxLib.h>
#include "SoundPlayLaunch.h"

// ����炵�����Ȃ��Ƃ��R�����g�A�E�g
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
	assert(ite != resourceSound_.end() || !"SoundPlayLaunch�ŋ�̃��\�[�X�ɎQ�Ƃ��悤�Ƃ��܂���");

	if (IsPlaySound(id) && !isReplay)return;

	//DX_PLAYTYPE_NORMAL�@:�@�m�[�}���Đ�
	//DX_PLAYTYPE_BACK�@�@ : �o�b�N�O���E���h�Đ�
	//DX_PLAYTYPE_LOOP�@�@ : ���[�v�Đ�

	if (type != DX_PLAYTYPE_NORMAL &&
		type != DX_PLAYTYPE_BACK   &&
		type != DX_PLAYTYPE_LOOP)
	{
		assert(!"�����ȉ��Đ��^�C�v�ł�");
	}

#ifdef DEBUG_SOUND
	PlaySoundMem(resourceSound_.at(id), type);
#endif
}

void SoundPlayLaunch::Stop(const SoundResourceManager::SOUND_RESOURCE_ID& id)
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunch�ŋ�̃��\�[�X�ɎQ�Ƃ��悤�Ƃ��܂���");

#ifdef DEBUG_SOUND
	StopSoundMem(resourceSound_.at(id));
#endif
}

const bool SoundPlayLaunch::IsPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id)
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunch�ŋ�̃��\�[�X�ɎQ�Ƃ��悤�Ƃ��܂���");

	return CheckSoundMem(resourceSound_.at(id)) > 0 ? true : false;
}

const int SoundPlayLaunch::GetHandleId(const SoundResourceManager::SOUND_RESOURCE_ID& id)const
{
	const auto& ite = resourceSound_.find(id);
	assert(ite != resourceSound_.end() || !"SoundPlayLaunch�ŋ�̃��\�[�X�ɎQ�Ƃ��悤�Ƃ��܂���");

	return resourceSound_.at(id);
}
