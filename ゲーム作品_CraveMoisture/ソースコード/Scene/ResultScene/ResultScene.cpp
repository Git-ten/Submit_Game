#include "../../APPLICATION/Application.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::LoadSceneData(void)
{
	gameToResultData_ = SceneManager::GetInstance().GetGameToResultSceneData();

	LoadData::LoadDataInfo info = { "ResultSceneInfo.json", "Numbers" };
	InitNumbersImage(info);

	// ResultDamage
	info.dataName = "Result_Damage";
	imgResultDamage_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::RESULT_DAMAGE);

	// ResultTime
	info.dataName = "Result_Time";
	imgResultTime_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::RESULT_TIME);

	info.dataName = "Level";
	imgResultLevel_ = std::make_unique<Image>(info, gameToResultData_.levelResource);

	info.dataName = "Result_HeroMovie";
	playerMovie_ = std::make_unique<Movie>(info, ResourceManager::RESOURCE_ID::RESULT_MOVIE_HERO);
	playerMovie_->Play(DX_PLAYTYPE_BACK);
}

void ResultScene::InitSpecial(void)
{
}

void ResultScene::UpdateSpecial(void)
{
	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("decision"))
		nextSceneId_ = SceneID::TITLE;
}

void ResultScene::DrawBack(void)
{
	playerMovie_->DrawMovie();

	//­‚µˆÃ“]
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, screenSize.x / 2, screenSize.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ResultScene::DrawSpecial(void)
{
	imgResultTime_->DrawImage();
	imgResultDamage_->DrawImage();

	DrawTime(gameToResultData_.resultTime);
	DrawDamage(gameToResultData_.resultDamage);

	imgResultLevel_->DrawImage();
}