#pragma once
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Common/DataLoad/DataImage.h"

/// <summary>
/// �� ����N���X�ł͂��邪�A�{���I�ɂ͉摜���̗���ł��邽�߁A
///    Image�Ɠ����̈����ł���
/// </summary>

class Movie
{
public:

	// �R���X�g���N�^
	// �Ăяo�������_�ŉ摜�f�[�^�����[�h����
	Movie(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	// �f�X�g���N�^
	~Movie(void);

	// ����Đ�(�����ōĐ��^�C�v��I��)
	void Play(int type);
	// ����̒�~
	void Stop(void);

	// �`��
	void DrawMovie(void);

private:

	// �f�[�^�̏��
	// ID�����͓���̂���
	DataImage movieInfo_;

};