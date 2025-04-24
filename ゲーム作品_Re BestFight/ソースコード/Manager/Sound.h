#pragma once
#include<string>

class Sound
{
public:

	enum class TYPE
	{
		NONE,
		MUSIC,//BGM
		WAVE,//���ʉ�
	};

	//�R���X�g���N�^
	Sound(void);
	//�R���X�g���N�^
	Sound(const TYPE& type, std::string path);

	//�J������
	void Release(void);

	//�ǂݍ��ݏ���
	void Load(void);

	//�G�t�F�N�g���v���C
	void Play(int type);
	//�G�t�F�N�g���X�g�b�v
	void Stop(void);
	//�Đ��������`�F�b�N
	bool IsCheckPlay(void)const;

private:

	//�T�E���h�̎��
	TYPE type_;

	//�T�E���h�̃p�X
	std::string path_;

	//�T�E���h�̃n���h��
	int handleId_;

};

