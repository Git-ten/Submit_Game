#include "../../Scene/TitleScene/TitleScene.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../Scene/TutorialScene/TutorialScene.h"
#include "../../Scene/ResultScene/ResultScene.h"
#include "../../Scene/SelectLevelScene/SelectLevelScene.h"
#include "SceneStatus.h"

SceneStatus::SceneStatus(void):
	nowSceneId_(SceneID::SCENE_ID::NONE)
{
}

void SceneStatus::InitScene()
{
	scene_->Init();
}

void SceneStatus::UpdateScene(void)
{
	scene_->Update();
}

void SceneStatus::DrawScene(void)
{
	scene_->Draw();
}

const SceneID::SCENE_ID& SceneStatus::GetNowSceneState(void) const
{
	return nowSceneId_;
}

void SceneStatus::ChangeSceneState(const SceneID::SCENE_ID& id)
{
	nowSceneId_ = id;

	if (scene_ != nullptr)
		scene_->Release();

	switch (nowSceneId_)
	{
	case SceneID::SCENE_ID::NONE:
		break;
	case SceneID::SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		break;
	case SceneID::SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>();
		break;
	case SceneID::SCENE_ID::TUTORIAL:
		scene_ = std::make_unique<TutorialScene>();
		break;
	case SceneID::SCENE_ID::RESULT:
		scene_ = std::make_unique<ResultScene>();
		break;
	case SceneID::SCENE_ID::SELECT:
		scene_ = std::make_unique<SelectLevelScene>();
		break;
	default:
		assert(!"SCENE_ID‚É‘Î‰ž‚·‚éƒNƒ‰ƒX‚ª‚ ‚è‚Ü‚¹‚ñ");
		break;
	}
}