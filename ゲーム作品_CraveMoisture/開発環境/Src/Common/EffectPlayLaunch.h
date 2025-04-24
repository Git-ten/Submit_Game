#pragma once
#include <map>
#include "../Utility/VectorHeader.h"
#include "../Application/SINGLETON_MANAGER/EffectResourceManager/EffectResourceManager.h"
#include "DataLoad/DataEffect.h"

class EffectPlayLaunch
{
public:

	// �R���X�g���N�^
	EffectPlayLaunch(void);
	// �f�X�g���N�^
	~EffectPlayLaunch(void);

	// �Đ�����G�t�F�N�g��o�^
	void AddPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const int handleId);

	// �v���C
	void Play(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const DataEffect& data);

	// �Ǐ]��ԂōĐ�����(�����ŎQ�Ƃ�����)
	void FollowPlay(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const DataEffect& data,
		Position3DF& pos, Rot3DF* rot = nullptr);

	// �Ǐ]����(FollowPlay�ōĐ�����Ă���Ƃ��̂�)
	void UpdateFollow(void);

	// �X�g�b�v
	void Stop(const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// �Đ����̃G�t�F�N�g�����ׂĒ�~
	void AllStop(void);

	// �Đ���Ԃ��擾
	const bool IsPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

	// ���W�����������ɓ�����
	void MoveEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Dir3DF& dir, const float& distance);

	// �v���CID���擾
	const int GetPlayId(const EffectResourceManager::EFFECT_RESOURCE_ID& id)const;

	// ���W�̎擾
	const Position3DF& GetPos(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

	// �Đ����x���擾
	const float GetSpeed(const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// �X�P�[���̎擾
	const Size3DF& GetScl(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

	// ���W���Z�b�g
	void SetPos(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Position3DF& pos);
	// ��]���Z�b�g
	void SetRot(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Rot3DF& rot);
	// �X�P�[�����Z�b�g
	void SetScl(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Size3DF& scl);
	// �Đ����x���Z�b�g
	void SetSpeed(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const float speed);

private:

	using RESOURCE_EFFECT = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, int>;
	using PLAY_EFFECT = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, int>;
	using EFFECT_POS = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Position3DF>;
	using EFFECT_SPEED = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, float>;
	using EFFECT_SCL = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Size3DF>;
	using FOLLOW_EFFECT_POS = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Position3DF*>;
	using FOLLOW_EFFECT_ROT = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Rot3DF*>;

	// �G�t�F�N�g�̏��
	struct EFFECT_INFO
	{
		// ���\�[�X
		RESOURCE_EFFECT resourceEffects;
		// �v���C�n���h��
		PLAY_EFFECT playEffects;

		// ���ݍ��W
		EFFECT_POS pos;

		// ���݂̍Đ����x
		EFFECT_SPEED speed;

		// ���݂̃X�P�[��
		EFFECT_SCL scl;

		// �Ǐ]�p::�Ǐ]���W�̎Q��
		FOLLOW_EFFECT_POS followPos;

		FOLLOW_EFFECT_ROT followRot;
	};

private:

	// �G�t�F�N�g
	EFFECT_INFO effect_;

	// �G�t�F�N�g�̓o�^�`�F�b�N
	void CheckPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

};