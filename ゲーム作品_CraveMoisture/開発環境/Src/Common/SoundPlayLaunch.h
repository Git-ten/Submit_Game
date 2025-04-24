#pragma once
#include <map>
#include "../APPLICATION/SINGLETON_MANAGER/SoundResourceManager/SoundResourceManager.h"

class SoundPlayLaunch
{
public:

	// �R���X�g���N�^
	SoundPlayLaunch(void);
	// �f�X�g���N�^
	~SoundPlayLaunch(void);

	// �Đ�����G�t�F�N�g��o�^
	void AddPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id, const int handleId);

	// �v���C
	void Play(const SoundResourceManager::SOUND_RESOURCE_ID& id, int type, bool isReplay = false);

	// �X�g�b�v
	void Stop(const SoundResourceManager::SOUND_RESOURCE_ID& id);

	// �Đ���Ԃ��擾
	const bool IsPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id);

	// �w�肷�鉹���\�[�X�̃n���h��ID���擾
	const int GetHandleId(const SoundResourceManager::SOUND_RESOURCE_ID& id)const;

private:

	using RESOURCE_SOUND = std::map<SoundResourceManager::SOUND_RESOURCE_ID, int>;

private:

	RESOURCE_SOUND resourceSound_;

};