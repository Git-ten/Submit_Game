#pragma once
#include <memory>
#include <functional>
#include "Image.h"

class ImageGUI
{
public:

	// �R���X�g���N�^
	ImageGUI(const std::function<bool(void)>&conditionFunc, const std::function<void(void)>& runFunc);
	// �f�X�g���N�^
	~ImageGUI(void);

	// GUI�Ƃ��Ďg�p����摜��o�^
	void SetImage(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);

	// GUI�̎��s����������ɒǉ�
	void AddCondition(const std::function<bool(void)>& conditionFunc);

	// GUI�̍X�V(true��Running)
	void UpdateGUI(void);

	// GUI�̕`��
	void DrawGUI(void);

	// �����蔻��p�̘g���擾
	const Plane2D& GetCollPlane(void)const;

	// �Z�b�g����Ă���摜�̍��W���擾
	const Position2D& GetPos(void)const;

private:

	using CONDITION = std::function<bool(void)>;
	using RUN = std::function<void(void)>;

private:

	// �摜�̃C���X�^���X
	std::unique_ptr<Image>images_;

	// ���s����
	CONDITION condition_;
	// ���s�֐�
	RUN run_;

};