#pragma once
#include <string>
#include <vector>

class Resource
{
public:
	
	// ���\�[�X�^�C�v
	enum class RESOURCE_TYPE
	{
		NONE,
		IMG,	//�ʏ�̉摜
		NORM,	//�@���}�b�v
		IMGS,	//�����摜
		MODEL,	//3D���f��
		MOVIE,	//����
	};

	// �R���X�g���N�^
	Resource(void);
	// �R���X�g���N�^
	explicit Resource(const RESOURCE_TYPE& type, const std::string& path);
	// �R���X�g���N�^(IMGS�p)
	explicit Resource(const RESOURCE_TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY);
	// �f�X�g���N�^
	~Resource(void);


	// ���
	void Release(void);
	//�����J��
	void Release(int handleId);


	// �ǂݍ���
	void Load(void);


	// �����摜�n���h����ʔz��ɃR�s�[
	void CopyHandle(int* imgs);


	// ���\�[�X�^�C�v
	RESOURCE_TYPE resType_;


	// ���\�[�X�̓ǂݍ��ݐ�
	std::string path_;


	// �摜�ƃ��f���̃n���h��ID
	int handleId_;


	// IMGS::LoadDivGraph�p
	std::vector<int>handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;


	// ���f�������p
	std::vector<int> duplicateModelIds_;

};

