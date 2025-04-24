#pragma once
#include<string>
#include"../Utility/VectorHeader.h"

class Effect
{
public:

	//�G�t�F�N�g�̎��
	enum class TYPE
	{
		NONE,
		EFFKSEER3D,//�G�t�F�N�V�A2D
		EFFKSEER2D,//�G�t�F�N�V�A3D
	};

	//�R���X�g���N�^
	Effect(void);
	Effect(const TYPE& type, std::string path);

	//�f�X�g���N�^
	~Effect(void);

	//�������J��
	void Release(void);

	//�ǂݍ���
	void Load(void);

	//�G�t�F�N�g���v���C
	void Play(void);
	//�G�t�F�N�g���X�g�b�v
	void Stop(void);
	//�Đ��������`�F�b�N
	bool IsCheckPlay(void)const;

	//�G�t�F�N�g�̏����Z�b�g
	void SetPos(Position3DF pos);
	void SetRot(Rot3DF rot);
	void SetScl(Size3DF scl);
	void SetSpeed(float speed);
	void SetRGB(int r, int g, int b, int a);
	Position3DF GetPos(void)const;

private:

	//�G�t�F�N�g�̎��
	TYPE type_;
	//�G�t�F�N�g�f�[�^�p�X
	std::string path_;
	//���\�[�X
	int resHandleId_;
	//�v���C
	int playHandleId_;

	//�G�t�F�N�g�̏��
	Position3DF pos_;

};