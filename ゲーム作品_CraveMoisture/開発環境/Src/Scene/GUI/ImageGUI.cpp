#include <cassert>
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "ImageGUI.h"

ImageGUI::ImageGUI(const std::function<bool(void)>& conditionFunc, const std::function<void(void)>& runFunc):
	condition_(conditionFunc),
	run_(runFunc)
{
}

ImageGUI::~ImageGUI(void)
{
}

void ImageGUI::SetImage(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id)
{
	images_ = std::make_unique<Image>(info, id);

	// �����蔻��p�̘g�̃Z�b�e�B���O
	images_->SetUpCollision();
}

void ImageGUI::AddCondition(const std::function<bool(void)>& conditionFunc)
{
	std::function<bool(void)>temp = condition_;
	condition_ = [=]() { return conditionFunc() && temp(); };
}

void ImageGUI::UpdateGUI(void)
{
	assert(images_ != nullptr || !"GUI�͉摜��K���Z�b�g���Ȃ��Ƃ����܂���");
	if (SceneManager::GetInstance().IsEndGame())return;

	// �����𖞂���������s
	if (condition_())
		run_();
}

void ImageGUI::DrawGUI(void)
{
	images_->DrawImage();
}

const Plane2D& ImageGUI::GetCollPlane(void) const
{
	return images_->GetCollPlane();
}

const Position2D& ImageGUI::GetPos(void) const
{
	assert(images_ != nullptr || !"�摜���o�^����Ă��Ȃ��̂ɍ��W���擾���悤�Ƃ��܂���");

	return images_->GetCenterPos();
}