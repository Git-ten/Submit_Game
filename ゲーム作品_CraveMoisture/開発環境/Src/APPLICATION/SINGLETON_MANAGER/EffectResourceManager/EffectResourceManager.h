#pragma once
#include <map>
#include "EffectResource.h"

class EffectResourceManager
{
public:

	// �G�t�F�N�g���\�[�X��ID
	enum class EFFECT_RESOURCE_ID
	{
		SMOKE,						// �y���G�t�F�N�g
		BLACK_SMOKE,				// �����G�t�F�N�g
		SLASH_RIGHT,				// ���̃G�t�F�N�g(�E)
		SLASH_LEFT,					// ���̃G�t�F�N�g(��)
		SLASH_HORIZ,				// ���̃G�t�F�N�g(��)
		SLASH_DUO,					// ���̃G�t�F�N�g(�o)
		SLASH_UP,					// ���̃G�t�F�N�g(��)
		SLASH_TORIO,				// ���̃G�t�F�N�g(�O�d�t)
		SLASH_DEVIATION,			// ���̃G�t�F�N�g(��)
		SLASH_ACCEL,				// ���̃G�t�F�N�g(�A�N�Z��)
		SLASH_FLASH,				// ���̃G�t�F�N�g(��M)
		
		ENEMY_ATTACK_READY,			// �u�C�r���E�S�A�v�̍U�������G�t�F�N�g
		EVIL_GOA_CLAW,				// �u�C�r���E�S�A�v�̒܃G�t�F�N�g
		EVIL_GOA_CLAW_RIGHT,		// �u�C�r���E�S�A�v�̒܃G�t�F�N�g(�E)
		EVIL_GOA_CLAW_LEFT,			// �u�C�r���E�S�A�v�̒܃G�t�F�N�g(��)
		EVIL_GOA_CLAW_DUO,			// �u�C�r���E�S�A�v�̒܃G�t�F�N�g(�o)
		EVIL_GOA_CLAW_DESTROY,		// �u�C�r���E�S�A�v�̒܃G�t�F�N�g(�E)
		BOMB,						// �u�C�r���E�S�A�v�̔��j�G�t�F�N�g
		EVIL_GOA_AWAKE_1,			// �u�C�r���E�S�A�v�̊o���G�t�F�N�g_1
		EVIL_GOA_AWAKE_2,			// �u�C�r���E�S�A�v�̊o���G�t�F�N�g_2
		EVIL_GOA_AWAKE_ATTACK_1,	// �u�C�r���E�S�A�v�̊o���U���G�t�F�N�g_1
		EVIL_GOA_AWAKE_ATTACK_2,	// �u�C�r���E�S�A�v�̊o���U���G�t�F�N�g_2

		SPARK,						// �ΉԃG�t�F�N�g
		SONIC_BOOM,					// �\�j�b�N�u�[��
		LIGHTNING,					// ���
		THUNDER_BOLT,				// �T���_�[�{���g(��)
		THUNDER_BOLT_ONE,			// �T���_�[�{���g(�v)
		STAGE_FIRE,					// �R����X�e�[�W�G�t�F�N�g
		CHARGE,						// �`���[�W���G�t�F�N�g
		CHARGE_FINISH,				// �`���[�W���G�t�F�N�g�̏I���G�t�F�N�g
		REPAIR,						// �e�N�j�b�N�Z::�񕜃G�t�F�N�g
		PHANTOM_BULLET,				// ���e�e��
		DARK_BULLET,				// �Í��e��

		PARYS,						// �p���B�G�t�F�N�g

		DAMAGE_SPARK,				// �_���[�W�G�t�F�N�g
		PLAYER_DAMAGE_SPARK,		// �v���C���[�p�̃_���[�W�G�t�F�N�g

		// �V�K�G�t�F�N�g
		HIGHT_POLI_EXPLOSION,		// �n�C�|���S�������G�t�F�N�g
		LOW_POLI_EXPLOSION,			// ���E�|���S�������G�t�F�N�g


		PLAYER_BULLET,			    // �v���C���[�̏e�e
	};

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static EffectResourceManager& GetInstance(void);

	// ������
	void Init(void);

	//�X�V
	void Update(void);
	//2D�G�t�F�N�g�̕`��
	void Draw2DEffect(void);
	//3D�G�t�F�N�g�̕`��
	void Draw3DEffect(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);
	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���[�h���āA�n���h��ID���擾
	EffectResource Load(const EFFECT_RESOURCE_ID& id);
	
	// ���[�h�����G�t�F�N�g�̍Đ��^�C�v(EFFECT_TYPE)���擾
	const EffectResource::EFFECT_TYPE GetEffectType(const EFFECT_RESOURCE_ID& id)const;

	// NULL�ł��邩
	static const bool IsNullptr(void);

private:

	// ���\�[�X�f�[�^
	using ResourcesData = std::map<EFFECT_RESOURCE_ID, EffectResource>;
	// ���[�h�ς݃f�[�^
	using LoadedData = std::map<EFFECT_RESOURCE_ID, EffectResource>;

private:

	// �ÓI�C���X�^���X
	static EffectResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	ResourcesData resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	LoadedData loadedMap_;

	// private�ŃR���X�g���N�^�𐶐�����
	EffectResourceManager(void);
	// �R�s�[�R���X�g���N�^�����l
	EffectResourceManager(const EffectResourceManager&);
	//�f�X�g���N�^
	~EffectResourceManager(void) = default;


	// �������[�h
	EffectResource _Load(const EFFECT_RESOURCE_ID& id);

};

