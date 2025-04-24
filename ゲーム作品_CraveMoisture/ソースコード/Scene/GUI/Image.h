#pragma once
#include <string>
#include <functional>
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Common/DataLoad/DataImage.h"

class Image
{
public:

	// �摜�̎��
	enum IMAGE_TYPE
	{
		NORMAL,
		GAGE,
		CIRCLE_GAGE,
	};

	// �R���X�g���N�^
	// �Ăяo�������_�ŉ摜�f�[�^�����[�h����
	Image(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id,
		const IMAGE_TYPE& type = IMAGE_TYPE::NORMAL, 
		const int addId = -1);
	// �f�X�g���N�^
	~Image(void);

	// �Q�[�W�̃��[�g�̎Q�Ƃ��Z�b�g�A�b�v
	void SetUpImageGageRate(float& gage, float maxGage, Color3D color, float addAngle = 0.0f);

	// �Q�[�W�ɉ��o�p�̃p�����[�^���Z�b�g
	void SetUpImageEventGageRate(float gage);

	// �A�j���[�V����
	// �����Ŏ��s
	void AnimationImage(const std::function<void(void)>&animation);

	// �摜�̕`��
	void DrawImage(int modelId = -1);
	// �摜�̋�`�؂���`��
	void DrawRectImage(const Position2D& rightPos, const Size2D& size);

	// �摜�̃Q�[�W�`��
	void DrawGageImage(const Color3D& color = { 255, 255, 255});

	// �摜�̉~�`�؂���`��
	void DrawCircleImage(const float rate);

	// ��ނɉ������摜�̕`��
	void DrawTypeImage(void);

	// �����x���Z�b�g
	void SetAlpha(const float alpha);

	// �����蔻��p�̘g���Z�b�g�A�b�v
	void SetUpCollision(void);

	// ���W�̃Z�b�g
	void SetPos(const Position2D& pos);
	
	// �g�p���Ă���摜�̎擾
	const ResourceManager::RESOURCE_ID& GetNowId(void)const;

	// �摜�̕ύX
	void ChangeImage(const ResourceManager::RESOURCE_ID& id);

	// �摜�̃n���h��ID
	const int GetId(void)const;

	// �摜�̍�����W
	const Position2D& GetLeftUpPos(void)const;
	// �摜�̉E����W
	const Position2D& GetRightDownPos(void)const;
	// �摜�̒��S���W
	const Position2D& GetCenterPos(void)const;
	// �摜�̃T�C�Y
	const Size2D GetSize(void);

	// �摜�̊g�嗦
	const double GetScl(void)const;
	// �摜�̃A���O��
	const double GetAngle(void)const;

	// �����蔻��p�̘g���擾
	const Plane2D& GetCollPlane(void)const;

private:

	IMAGE_TYPE imageType_;
	float* imageGageRate_;
	float prevImageGageRate_;
	float imageGageMaxRate_;
	float addImageGageAngle_;
	Color3D imageGageColor_;
	int addResource_;

	// �f�[�^�̏��
	DataImage imgInfo_;

	// ���\�[�XID
	ResourceManager::RESOURCE_ID resId_;

	// �摜�̃A���t�@�l
	float alpha_;

};