#pragma once
#include <string>
#include <vector>

class EffectResource
{
public:

	// ���\�[�X�^�C�v
	enum class EFFECT_TYPE
	{
		NONE,
		EFFEKSEER_3D,	// �G�t�F�N�V�A3D
		EFFEKSEER_2D,	// �G�t�F�N�V�A2D
		SPLITE,			// �X�v���C�g�A�j���G�t�F�N�g
	};

	// �R���X�g���N�^
	EffectResource(void);
	// �R���X�g���N�^
	explicit EffectResource(const EFFECT_TYPE& type, const std::string& path);
	explicit EffectResource(const EFFECT_TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY);
	// �f�X�g���N�^
	~EffectResource(void);


	// ���
	void Release(void);
	//�����J��
	void Release(int handleId);


	// �ǂݍ���
	void Load(void);


	// ���\�[�X�^�C�v
	EFFECT_TYPE resType_;


	// ���\�[�X�̓ǂݍ��ݐ�
	std::string path_;


	// �G�t�F�N�g�̃n���h��ID
	int handleId_;
	
	// Splite::LoadDivGraph�p
	std::vector<int>handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;


	// �G�t�F�N�g�����p
	std::vector<int> duplicateEffectIds_;

};