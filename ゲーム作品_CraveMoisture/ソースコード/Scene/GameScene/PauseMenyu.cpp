#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneID.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../../Common/Collider.h"
#include "PauseMenyu.h"

PauseMenyu::PauseMenyu(void):
	selectKind_(SELECT_KIND::NONE)
{
}

PauseMenyu::~PauseMenyu(void)
{
}

void PauseMenyu::LoadSceneData(void)
{
	LoadData::LoadDataInfo info = { "GameSceneInfo.json", "PauseBack" };

	imgPauseBack_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::PAUSE_GAME_BACK);
	imgPauseBack_->SetAlpha(0.7f);// “§–¾“x

	info.dataName = "ReturnTitle";
	SetUpGUI(info, ResourceManager::RESOURCE_ID::PAUSE_RETURN_TITLE,
		[&]() { return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::RETURN_TITLE; },
		[&]() { nextSceneId_ = SceneID::TITLE; });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[0]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::RETURN_TITLE;},
		[&]() { imgCircle_->SetPos(guiImages_[0]->GetPos());
				selectKind_ = SELECT_KIND::RETURN_TITLE; });

	info.dataName = "ReturnGame";
	SetUpGUI(info, ResourceManager::RESOURCE_ID::PAUSE_RETURN_GAME,
		[&]() { return SimpleInput::GetInstance().GetInput("decision") && selectKind_ == SELECT_KIND::RETURN_GAME; },
		[&]() { SetMenyuIsOpen(false); });

	SetUpGUI(info, ResourceManager::RESOURCE_ID::NONE,
		[&]() { return Collider::CollisionRectToMouse(guiImages_[3]->GetCollPlane(),
			InputManager::GetInstance().GetMousePos()) || selectKind_ == SELECT_KIND::RETURN_GAME;},
		[&]() { imgCircle_->SetPos(guiImages_[3]->GetPos());
	selectKind_ = SELECT_KIND::RETURN_GAME; });




	info.dataName = "SelectCircle";
	imgCircle_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SELECT_CIRCLE);

	info.dataName = "Time";
	imgTime_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::RESULT_TIME);
	info.dataName = "Damage";
	imgDamage_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::RESULT_DAMAGE);

	InitNumbersImage(info);
}

void PauseMenyu::InitSpecial(void)
{
}

void PauseMenyu::UpdateSpecial(void)
{
	auto& input = SimpleInput::GetInstance();

	selectKind_ = static_cast<SELECT_KIND>(
		InputSelectMenyu(input.GetInput("selectUp"), input.GetInput("selectDown"),
			static_cast<int>(selectKind_),
			static_cast<int>(SELECT_KIND::NONE), static_cast<int>(SELECT_KIND::MAX)));
}

void PauseMenyu::InitOpenMenyu(void)
{
	selectKind_ = SELECT_KIND::NONE;

	if (isGuiDraw_)
	{
		SetMouseDispFlag(true);
		SceneManager::GetInstance().LockSceneUpdate();
	}
	else
	{
		SetMouseDispFlag(false);
		SceneManager::GetInstance().UnLockSceneUpdate();
	}
}

void PauseMenyu::InitSoundPlayLaunch(void)
{
}

void PauseMenyu::DrawBack(void)
{
	if (!isGuiDraw_)return;

	imgPauseBack_->DrawImage();

	if (selectKind_ != SELECT_KIND::NONE)imgCircle_->DrawImage();

	imgTime_->DrawImage();
	imgDamage_->DrawImage();

	SceneManager::GameSceneToResultScene data = SceneManager::GetInstance().GetGameToResultSceneData();

	DrawTime(data.resultTime);
	DrawDamage(data.resultDamage);
}