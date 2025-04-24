#include "SelectLevelScene.h"

SelectLevelScene::SelectLevelScene(void)
{
}

SelectLevelScene::~SelectLevelScene(void)
{
}

void SelectLevelScene::LoadSceneData(void)
{
	selectMenyu_ = std::make_unique<SelectLevelMenyu>();
	selectMenyu_->Init();
}

void SelectLevelScene::UpdateSpecial(void)
{
	selectMenyu_->Update();
}

void SelectLevelScene::DrawSpecial(void)
{
	selectMenyu_->Draw();
}