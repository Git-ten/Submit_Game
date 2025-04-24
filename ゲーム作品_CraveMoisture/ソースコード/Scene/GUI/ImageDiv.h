#pragma once
#include "../../APPLICATION/SINGLETON_MANAGER/EffectResourceManager/EffectResourceManager.h"
#include "../../Common/DataLoad/DataImageDiv.h"
#include "../../Utility/VectorHeader.h"
#include "Image.h"

class ImageDiv
{
public:

	// �R���X�g���N�^
	// �Ăяo�������_�ŉ摜�f�[�^�����[�h����
	ImageDiv(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	ImageDiv(const LoadData::LoadDataInfo& info, const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// �f�X�g���N�^
	~ImageDiv(void);

	// �w�肵���摜�̕`��
	void DrawImageDiv(const int number);

	// �w�肵���摜���w��̍��W�ɕ`��
	void DrawImageDiv(const int number, const Position2D& pos);

	void SetPos(const Position2D& pos);

	// �摜�̍��W���擾
	const Position2D& GetPos(void)const;
	// �摜�̍��W���擾(�Q��)
	Position2D& GetPosReference(void);

	// ��`�����Q��
	Plane2D& GetPlaneReference(void);


	// �摜�̃T�C�Y���擾
	Size2D GetSize(void)const;

	// �摜�̖������擾
	int GetImageDataSize(void);

private:

	// �摜�̏��
	DataImageDiv imgInfos_;

};