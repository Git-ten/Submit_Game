#pragma once
#include <string>

class SoundResource
{
public:

	// �R���X�g���N�^
	SoundResource(void);
	// �R���X�g���N�^
	explicit SoundResource(const std::string& path);
	// �f�X�g���N�^
	~SoundResource(void);

	// ���
	void Release(void);
	//�����J��
	void Release(int handleId);


	// �ǂݍ���
	void Load(void);


	// ���\�[�X�̓ǂݍ��ݐ�
	std::string path_;


	// �T�E���h�̃n���h��ID
	int handleId_;

};