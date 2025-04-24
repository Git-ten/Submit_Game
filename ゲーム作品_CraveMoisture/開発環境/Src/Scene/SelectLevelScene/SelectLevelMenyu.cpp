#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../../Common/Collider.h"
#include "../../ACTOR/ActorBase.h"
#include "SelectLevelMenyu.h"

SelectLevelMenyu::SelectLevelMenyu(void):
	selectKind_(SELECT_KIND::EASY)
{
}

SelectLevelMenyu::~SelectLevelMenyu(void)
{
}

void SelectLevelMenyu::LoadSceneData(void)
{
	LoadData::LoadDataInfo info = { "SelectLevelInfo.json", "SelectLevelBack" };
	imgSelectLevelBack_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SELECT_LEVEL_BACK);

	info.dataName = "SelectEnemyEvilGoa";
	imgEnemyIcon_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SELECT_ENEMY_EVILGOA);

	info.dataName = "SelectCircle";
	imgCircle_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SELECT_CIRCLE);



	info.dataName = "LevelEasy";
	SetUpGUI(info, ResourceManager::RESOURCE_ID::LEVEL_EASY,
		[&]() {return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::EASY; },
		[&]() { SceneManager::GetInstance().SetUpSelectToGameScene({ ActorBase::ENEMY_LEVEL_EASY });
			nextSceneId_ = SceneID::GAME; });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[0]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::EASY;},
		[&]() { imgCircle_->SetPos(guiImages_[0]->GetPos());
				selectKind_ = SELECT_KIND::EASY;});



	info.dataName = "LevelNormal";
	SetUpGUI(info, ResourceManager::RESOURCE_ID::LEVEL_NORMAL,
		[&]() { return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::NORMAL; },
		[&]() { SceneManager::GetInstance().SetUpSelectToGameScene({ ActorBase::ENEMY_LEVEL_NORMAL });
			nextSceneId_ = SceneID::GAME; });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[2]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::NORMAL;},
		[&]() { imgCircle_->SetPos(guiImages_[2]->GetPos());
				selectKind_ = SELECT_KIND::NORMAL;});


	info.dataName = "LevelHard";
	SetUpGUI(info, ResourceManager::RESOURCE_ID::LEVEL_HARD,
		[&]() { return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::HARD; },
		[&]() { SceneManager::GetInstance().SetUpSelectToGameScene({ ActorBase::ENEMY_LEVEL_HARD });
			nextSceneId_ = SceneID::GAME; });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[4]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::HARD;},
		[&]() { imgCircle_->SetPos(guiImages_[4]->GetPos());
				selectKind_ = SELECT_KIND::HARD;});
}

void SelectLevelMenyu::InitSpecial(void)
{
	isGuiDraw_ = true;
}

void SelectLevelMenyu::UpdateSpecial(void)
{
	auto& input = SimpleInput::GetInstance();

	selectKind_ = static_cast<SELECT_KIND>(
		InputSelectMenyu(input.GetInput("selectUp"), input.GetInput("selectDown"),
			static_cast<int>(selectKind_),
			static_cast<int>(SELECT_KIND::NONE), static_cast<int>(SELECT_KIND::MAX)));
}

void SelectLevelMenyu::DrawBack(void)
{
	imgSelectLevelBack_->DrawImage();
	imgEnemyIcon_->DrawImage();
	imgCircle_->DrawImage();
}