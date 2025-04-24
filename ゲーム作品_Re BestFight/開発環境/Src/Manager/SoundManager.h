#pragma once
#include<memory>
#include<map>
#include<string>
#include"Sound.h"

class SoundManager
{
public:
	
	//���\�[�X��
	enum class SOD
	{
		TITLE_BGM,//�^�C�g��BGM
		BATTLE_BGM,//�o�g��BGM
		SELECT_BGM,//�Z���N�gBGM
		VICTORY,//���U���g��
		ATTACK_CRASH,//�N���b�V���U����
		ATTACK_STRONG,//���Ō�
		SWING,//��U�艹
		SORD_FINISH,//����
		SORD_HIT,//��������������
		THUNDER_BOLT,//����
		SKILL_FLAME,//���K�E�Z��
		SKILL_THUNDER,//���K�E�Z
		SKILL_MAX,//�K�E�Z�Q�[�W�}�b�N�X
		DECISION,//���艹
		DECISION_2,//���艹2
		DECISION_3,//���艹3
		CHANGE_SCENE,//�V�[���؂�ւ���
		JUST,//�W���X�g��
		FINISH,//��������
		METEO,//覐Ή�
		METEO_HIT,//覐΃q�b�g��
		SNIPE,//�X�i�C�v��
		GURD_BREAK,//�K�[�h�u���C�N
		QUICK_MOVE,//�����ړ�
		NOT_POSSIBLE,//�s��
		COUNT_DOWN,//�J�E���g�_�E��
		START,//�X�^�[�g��
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static SoundManager& GetInstance(void);

	//������
	void Init(void);
	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);
	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	std::weak_ptr<Sound> Load(const SOD& src);

private:

	using SoundsData = std::map<SOD, Sound>;
	using LoadedData = std::map<SOD, std::shared_ptr<Sound>>;


	//�ÓI�C���X�^���X
	static SoundManager* instance_;

	//���\�[�X�Ǘ��̑Ώ�
	SoundsData soundsMap_;

	//�ǂݍ��ݍς݃��\�[�X
	LoadedData loadedMap_;



	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SoundManager(void);
	//�R�s�[�R���X�g���N�^�����l
	SoundManager(const SoundManager&);
	// �f�X�g���N�^�����l
	~SoundManager(void) = default;

	// �������[�h
	std::weak_ptr<Sound> _Load(const SOD& sod);

};

