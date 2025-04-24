#pragma once
#include <map>
#include "../../Common/DataLoad/DataPaths.h"
#include "Resource.h"

class ResourceManager
{
public:

	// ���\�[�X��ID
	enum class RESOURCE_ID
	{
		NONE,
		TITLE_BACK,								// �^�C�g���w�i
		TITLE_NAME,								// �^�C�g����
		PUSH_ANY_BUTTON,						// PushAnyButton�摜
		SHUT_DOWN,								// �V���b�g�_�E���摜
		SHUT_DOWN_DELETE,						// �V���b�g�_�E���L�[�摜
		LOAD_BACK,								// ���[�h�w�i
		LOAD_GAGE,								// ���[�h�Q�[�W
		LOAD_GAGE_CASE,							// ���[�h�Q�[�W�̃P�[�X
		GAMEOVER_BACK,							// �Q�[���I�[�o�[�w�i
		NUMBERS,								// �����摜
		NUMBERS_RED,							// �����摜(�D)
		SYMBOLS,								// �L���摜
		
		MENU_CASE,								// ���j���[�g
		TUTORIAL_QUESTION,						// �`���[�g���A���ɐi�݂܂���
		YES,									// �͂�
		NO,										// ������
		SELECT_CIRCLE,							// �I�𒆂������~
		PAUSE_GAME_BACK,						// �ꎞ��~�̔w�i
		PAUSE_RETURN_TITLE,						// �u�^�C�g���ɖ߂镶���v
		PAUSE_RETURN_GAME,						// �u�Q�[�����ĊJ���镶���v
		RESULT_TIME,							// Result��ʂ́uTime�v�摜
		RESULT_DAMAGE,							// Result��ʂ́uDamage�v�摜

		SELECT_LEVEL_BACK,						// ��Փx�Z���N�g��ʂ̔w�i
		LEVEL_EASY,								// ��Փx::EASY
		LEVEL_NORMAL,							// ��Փx::NORMAL
		LEVEL_HARD,								// ��Փx::HARD
		SELECT_ENEMY_EVILGOA,					// �Z���N�g��ʂ́u�C�r���S�A�v�C���X�g

		SKYDOME_NIGHT,							// �X�J�C�h�[��::��
		SKYDOME_SUN,							// �X�J�C�h�[��::����
		SKYDONUT_SUN,							// �X�J�C�h�[�i�c::����
		DEFAULT_JET,							// �f�t�H���g�퓬�@
		ENEMY_DEFAULT_JET,						// �f�t�H���g�G�퓬�@
		ENEMY_MOB_01,							// ���u�G�l�~�[01
		ENEMY_MOB_02,							// ���u�G�l�~�[01
		ENEMY_BEE_BOSS,							// �I�^�G�l�~�[

		// �v���C���[���f��
		PLAYER_HERO,							// �v���C���[::�q�[���[

		// �G�l�~�[���f��
		ENEMY_DEMON,							// �G�l�~�[::�f�[����
		ENEMY_BEAR,								// �G�l�~�[::�x�A

		// �X�e�[�W���f��
		STAGE_RUINS,							// �X�e�[�W::�p��
		GATE_RUINS,								// �G���g�����X::�p�Зp
		STAGE_GROUND,							// �X�e�[�W::��n

		// ���탂�f��
		HERO_SWORD,								// �v���C���[�̌�

		// UI
		PLAYER_HP,								// �v���C���[UI::HP
		PLAYER_HP_CASE,							// �v���C���[UI::HP�̃P�[�X
		ENEMY_HP,								// �G�l�~�[UI::HP
		ENEMY_HP_CASE,							// �G�l�~�[UI::HP�̃P�[�X
		DOWN_GAGE,								// �_�E���Q�[�W
		DOWN_GAGE_CASE,							// �_�E���Q�[�W�̃P�[�X
		REPAIR_ICON,							// �񕜃e�N�j�b�N�Z�̃A�C�R��
		ICON_CASE,								// �e�N�j�b�N�Z�̃A�C�R���P�[�X
		EVIL_GOA_NAME,							// �u�C�r���S�A�v�̖��O�摜
		SKILL_GAGE,								// �v���C���[UI::�X�L���Q�[�W
		SKILL_GAGE_MAX,							// �v���C���[UI::�X�L���Q�[�W
		SKILL_GAGE_CASE,						// �v���C���[UI::�X�L���Q�[�W�P�[�X
		WILD_BEAR_NAME,							// �u���C���h�x�A�v�̖��O�摜

		// �V�F�[�_�Ɏg�p����e�N�X�`��
		SPHERE_METAL,							// ���^���X�t�B�A�}�b�v
		SWORD_METALIC,							// ���p�̃��^���b�N�e�N�X�`��
		SWORD_ROUGHNESS,						// ���p�̃��t�l�X�e�N�X�`��
		ROUGHNESS,								// �ʏ�̃��t�l�X�e�N�X�`��
		RUINS_ROUGHNESS,						// �p�Ѓ��t�l�X�e�N�X�`��
		HERO_ROUGHNESS,							// �v���C���[::�q�[���[���t�l�X�e�N�X�`��
		GROUND_ROUGHNESS,						// ���ނ烉�t�l�X�e�N�X�`��
		SWORD_LOCUS_TEXTURE,					// ���̃e�N�X�`��
		NOISE,									// �m�C�Y�e�N�X�`��
		CLAW_TEXTURE,							// �܂̃e�N�X�`��

		// ����
		RESULT_MOVIE_HERO,						// �v���C���[::�q�[���[�̃��U���g����


		// �`���[�g���A���p
		DELETE_TUTORIAL,						// �����{�^��
		TUTORIAL_UI,							// UI�`���[�g���A���摜
		TUTORIAL_CAMERA,						// �J�����`���[�g���A��
		TUTORIAL_MOVE,							// �ړ��`���[�g���A��
		TUTORIAL_JUMP,							// �W�����v�`���[�g���A��
		TUTORIAL_AVOID,							// ����`���[�g���A��
		TUTORIAL_ATTACK,						// �U���`���[�g���A��
		TUTORIAL_DASH,							// �_�b�V���`���[�g���A��
		TUTORIAL_CHARGE,						// �`���[�W�U���`���[�g���A��
		TUTORIAL_SPECIAL,						// �p���B�ƃW���X�g����̃`���[�g���A��
		TUTORIAL_JUST_AVOID_ATTACK,				// �W���X�g�����̓���`���[�W�U��
		TUTORIAL_TECHNIQUE,						// �e�N�j�b�N�Z�`���[�g���A��
		TUTORIAL_SKILL,							// �K�E�Z�`���[�g���A��
		TUTORIAL_BATTLE,						// ���C���o�g���`���[�g���A��
		TUTORIAL_END,							// �`���[�g���A���I��
		TUTORIAL_CAMERA_MOVIE,					// �J�����`���[�g���A������
		TUTORIAL_MOVE_MOVIE,					// �ړ��`���[�g���A������
		TUTORIAL_JUMP_MOVIE,					// �W�����v�`���[�g���A������
		TUTORIAL_AVOID_MOVIE,					// ����`���[�g���A������
		TUTORIAL_ATTACK_MOVIE,					// �U���`���[�g���A������
		TUTORIAL_DASH_MOVIE,					// �_�b�V���`���[�g���A������
		TUTORIAL_CHARGE_MOVIE,					// �`���[�W�`���[�g���A������
		TUTORIAL_SPECIAL_MOVIE,					// �p���B�ƃW���X�g����`���[�g���A������
		TUTORIAL_JUST_AVOID_ATTACK_MOVIE,		// �p���B�ƃW���X�g����`���[�g���A������
		TUTORIAL_TECHNIQUE_MOVIE,				// �e�N�j�b�N�Z�`���[�g���A������
		TUTORIAL_SKILL_MOVIE,					// �K�E�Z�`���[�g���A������
		TUTORIAL_BATTLE_MOVIE,					// ���C���o�g���`���[�g���A������
		TUTORIAL_MOUSE,							// �`���[�g���A���p�̃}�E�X�摜
		TUTORIAL_WASD_KEY,						// �`���[�g���A���p��WASD�L�[�摜
		TUTORIAL_R_KEY,							// �`���[�g���A���p��R�L�[�摜
		TUTORIAL_Q_KEY,							// �`���[�g���A���p��Q�L�[�摜
		TUTORIAL_SPACE_KEY,						// �`���[�g���A���p��Space�L�[�摜
		TUTORIAL_ALL_KEY,						// �`���[�g���A���p�̂��ׂẴL�[�摜

		// �R���g���[��
		TUTORIAL_D_PAD,							// �`���[�g���A���p�̏\���L�[�摜
		TUTORIAL_CONTOROLLER,					// �`���[�g���A���p�̃R���g���[���摜
		TUTORIAL_X_BUTTON,						// �`���[�g���A���p��X�L�[�摜
		TUTORIAL_A_BUTTON,						// �`���[�g���A���p��A�L�[�摜
		TUTORIAL_B_BUTTON,						// �`���[�g���A���p��B�L�[�摜

		TUTORIAL_CAMERA_EXPLAIN,				// �`���[�g���A���J�����̐�����
		TUTORIAL_MOVE_EXPLAIN,					// �`���[�g���A���ړ��̐�����
		TUTORIAL_JUMP_EXPLAIN,					// �`���[�g���A���W�����v�̐�����
		TUTORIAL_AVOID_EXPLAIN,					// �`���[�g���A������̐�����
		TUTORIAL_ATTACK_EXPLAIN,				// �`���[�g���A���U���̐�����
		TUTORIAL_DASH_EXPLAIN,					// �`���[�g���A���_�b�V���̐�����
		TUTORIAL_CHARGE_EXPLAIN,				// �`���[�g���A���`���[�W�̐�����
		TUTORIAL_SPECIAL_EXPLAIN,				// �`���[�g���A���p���B�ƃW���X�g����̐�����
		TUTORIAL_TECHNIQUE_EXPLAIN,				// �`���[�g���A���e�N�j�b�N�Z�̐�����
		TUTORIAL_SKILL_EXPLAIN,					// �`���[�g���A���K�E�Z�̐�����
		TUTORIAL_BATTLE_EXPLAIN,				// �`���[�g���A���o�g���̐�����
		TUTORIAL_JUST_AVOID_ATTACK_EXPLAIN,		// �`���[�g���A���W���X�g������̓���U���̐�����

	};

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);
	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���[�h���āA�n���h��ID���擾
	Resource Load(const RESOURCE_ID& id);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(const RESOURCE_ID& id);

	// NULL�ł��邩
	static const bool IsNullptr(void);

private:

	// ���\�[�X�f�[�^
	using ResourcesData = std::map<RESOURCE_ID, Resource>;
	// ���[�h�ς݃f�[�^
	using LoadedData = std::map<RESOURCE_ID, Resource>;

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	ResourcesData resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	LoadedData loadedMap_;

	// �摜�̒ǉ�(�L�[�{�[�h)
	void AddImageKeboard(const DataPaths& dataPath);
	// �摜�̒ǉ�(�R���g���[���p)
	void AddImageController(const DataPaths& dataPath);

	// private�ŃR���X�g���N�^�𐶐�����
	ResourceManager(void);
	// �R�s�[�R���X�g���N�^�����l
	ResourceManager(const ResourceManager&);
	//�f�X�g���N�^
	~ResourceManager(void) = default;


	// �������[�h
	Resource _Load(const RESOURCE_ID& id);
};