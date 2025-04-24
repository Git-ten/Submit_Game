#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/InputManager.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "../../Common/Collider.h"
#include "TitleMenyu.h"

TitleMenyu::TitleMenyu(void):
	selectKind_(SELECT_KIND::NONE)
{
}

TitleMenyu::~TitleMenyu(void)
{
}

void TitleMenyu::LoadSceneData(void)
{
	LoadData::LoadDataInfo info = { "TitleSceneInfo.json", "Yes" };

	// GUI‚Ì“o˜^
	SetUpGUI(info, ResourceManager::RESOURCE_ID::YES,
		[&]() { return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::YES; },
		[&]() { nextSceneId_ = SceneID::SCENE_ID::TUTORIAL; });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[0]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::YES;},
		[&]() { imgCircle_->SetPos(guiImages_[0]->GetPos());
			    selectKind_ = SELECT_KIND::YES; });


	info.dataName = "No";
	SetUpGUI(info, ResourceManager::RESOURCE_ID::NO,
		[&]() { return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::NO; },
		[&]() { nextSceneId_ = SceneID::SCENE_ID::SELECT; });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[2]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::NO;},
		[&]() { imgCircle_->SetPos(guiImages_[2]->GetPos());
			    selectKind_ = SELECT_KIND::NO; });

	info.dataName = "Tutorial_Question";
	imgTutorial_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::TUTORIAL_QUESTION);
	info.dataName = "MenuCase";
	imgMenu_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::MENU_CASE);

	info.dataName = "SelectCircle";
	imgCircle_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SELECT_CIRCLE);
}

bool TitleMenyu::IsCollMenyu(void)
{
	return Collider::CollisionRectToMouse(imgMenu_->GetCollPlane(),
		InputManager::GetInstance().GetMousePos());
}

Position2D TitleMenyu::GetPos(void)
{
	return imgMenu_->GetCenterPos();
}

void TitleMenyu::UpdateSpecial(void)
{
	auto& input = SimpleInput::GetInstance();

	selectKind_ = static_cast<SELECT_KIND>(
		InputSelectMenyu(input.GetInput("selectLeft"), input.GetInput("selectRight"),
		static_cast<int>(selectKind_),
		static_cast<int>(SELECT_KIND::NONE), static_cast<int>(SELECT_KIND::MAX)));
}

void TitleMenyu::InitOpenMenyu(void)
{
	selectKind_ = SELECT_KIND::NONE;
}

void TitleMenyu::InitSoundPlayLaunch(void)
{
}

void TitleMenyu::DrawBack(void)
{
	if (!isGuiDraw_)return;

	imgMenu_->DrawImage();
	imgTutorial_->DrawImage();

	if(selectKind_ != SELECT_KIND::NONE)imgCircle_->DrawImage();
}