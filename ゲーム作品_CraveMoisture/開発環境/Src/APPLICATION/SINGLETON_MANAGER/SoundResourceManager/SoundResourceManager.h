#pragma once
#include <map>
#include "SoundResource.h"

class SoundResourceManager
{
public:

	// �T�E���h���\�[�X
	enum class SOUND_RESOURCE_ID
	{
		// BGM
		TITLE,
		GAME,

		// ���ʉ�
		WALK,
		RUN,
		SWING,
		SWORD_ATTACK,
		DAMAGE,
		PARY,
		JUST_AVOID,
		DASH_SONIC,
		AVOID,
		JUMP,
		ELECTRO,
		THUNDER,
		SPARK,
		SLASH_FINISH,
		REPAIR,
		CHARGE,
		EVIL_GOA_SLASH,
		EVIL_GOA_BOM,
		EVIL_GOA_ROAR,
		EVIL_GOA_AWAKE_1,
		EVIL_GOA_AWAKE_2,
		METEO,

		ENEMY_FLINCH,
		ENEMY_DOWN,

		GAME_OVER,
	};

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static SoundResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);
	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���[�h���āA�n���h��ID���擾
	SoundResource Load(const SOUND_RESOURCE_ID& id);

	// NULL�ł��邩
	static const bool IsNullptr(void);

private:

	// ���\�[�X�f�[�^
	using ResourcesData = std::map<SOUND_RESOURCE_ID, SoundResource>;
	// ���[�h�ς݃f�[�^
	using LoadedData = std::map<SOUND_RESOURCE_ID, SoundResource>;

private:

	// �ÓI�C���X�^���X
	static SoundResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	ResourcesData resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	LoadedData loadedMap_;

	// private�ŃR���X�g���N�^�𐶐�����
	SoundResourceManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SoundResourceManager(const SoundResourceManager&);
	//�f�X�g���N�^
	~SoundResourceManager(void) = default;

	// �������[�h
	SoundResource _Load(const SOUND_RESOURCE_ID& id);

};