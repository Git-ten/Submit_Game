#pragma once
#include<memory>
#include <map>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		NONE,
		TITLE,//�^�C�g���摜
		PLAYGAME,//PlayGame
		PLAYGAME_IN,//PlayGame_Input
		ABOUTGAME,//AboutGame
		ABOUTGAME_IN,//AboutGameInput
		RESULT,//���U���g�摜
		EXPLAIN_1,//�Q�[�������摜_1
		EXPLAIN_2,//�Q�[�������摜_2
		EXPLAIN_3,//�Q�[�������摜_3
		EXPLAIN_4,//�Q�[�������摜_4
		EXPLAIN_4_2,//�Q�[�������摜_4_2
		EXPLAIN_4_3,//�Q�[�������摜_4_3
		EXPLAIN_5,//�Q�[�������摜_5
		EXPLAIN_5_2,//�Q�[�������摜_5_2
		EXPLAIN_5_3,//�Q�[�������摜_5_3
		EXPLAIN_6,//�Q�[�������摜_6
		EXPLAIN_6_2,//�Q�[�������摜_6_2
		EXPLAIN_6_3,//�Q�[�������摜_6_3
		EXPLAIN_6_4,//�Q�[�������摜_6_4
		EXPLAIN_EX,//�t�F�C�h���̃Q�[������
		MENYU,//���j���[�摜
		LEFT_KEY,//���{�^��
		RIGHT_KEY,//�E�{�^��
		MARK_1,//�v���C���[1�̈�
		MARK_2,//�v���C���[2�̈�
		PLAYER_FIST,//�v���C���[(�i����)
		PLAYER_SORD,//�v���C���[(���m)
		PLAYER_MAGIC,//�v���C���[(���p�t)
		STAGE,//�X�e�[�W
		SKY,//��摜
		SKYDOME_1,//�X�J�C�h�[��

		//UI
		HP_GAGE,//HP�Q�[�W
		B_HP_GAGE,//HP�Q�[�W(Big)
		STAMINA_GAGE,//�X�^�~�i�Q�[�W
		B_STAMINA_GAGE,//�X�^�~�i�Q�[�W(Big)
		CASE,//�P�[�X
		SKILL_GAGE,//�K�E�Z�Q�[�W
		B_SKILL_GAGE,//�K�E�Z�Q�[�W(Big)
		SKILL_CASE,//�K�E�Z�Q�[�W�P�[�X
		LEVEL_GAGE,//���x���Q�[�W
		LEVEL_ATTACK_GAGE,//�U���̓��x���Q�[�W
		B_LEVEL_ATTACK_GAGE,//�U���̓��x���Q�[�W(Big)
		LEVEL_DEFENCE_GAGE,//�h��̓��x���Q�[�W
		B_LEVEL_DEFENCE_GAGE,//�h��̓��x���Q�[�W(Big)
		LEVEL_AVOID_GAGE,//���̓��x���Q�[�W
		B_LEVEL_AVOID_GAGE,//���̓��x���Q�[�W(Big)
		MAGIC_GAGE,//���̓Q�[�W
		B_MAGIC_GAGE,//���̓Q�[�W(Big)
		SELECT_CARSOL,//�Z���N�g�J�[�\���摜
		SELECT,//�Z���N�g��ʉ摜
		START,//�J�n�{�^���摜
		START_INPUT,//�J�n�{�^�����͉摜
		CHAIN,//�`�F�[���摜
		RETURN_BUTTON,//�߂�{�^��
		RETURN_BUTTON_INPUT,//�߂�{�^���C���v�b�g
		RETURN_BUTTGAGE,//�߂�{�^���Q�[�W
		B_RETURN_BUTTGAGE,//�߂�{�^���Q�[�W(Big)
		SWITCH,//�d���{�^��
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);
	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);
	//�����J��
	void Release(const SRC& src, int handleId);
	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	std::weak_ptr<Resource> Load(const SRC& src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(const SRC& src);

private:

	using ResourcesData = std::map<SRC, Resource>;
	using LoadedData = std::map<SRC, std::shared_ptr<Resource>>;


	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	ResourcesData resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	LoadedData loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	// �R�s�[�R���X�g���N�^�����l
	ResourceManager(const ResourceManager&);
	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;



	//�����ł̂ݎg�p����֐�
	// �������[�h
	std::weak_ptr<Resource> _Load(const SRC& src);

};

