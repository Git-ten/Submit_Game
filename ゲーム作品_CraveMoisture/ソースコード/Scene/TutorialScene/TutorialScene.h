#pragma once
#include <functional>
#include <vector>
#include "../../Scene/GUI/Movie.h"
#include "../GameScene/GameScene.h"

class TutorialScene : public GameScene
{
public:

	TutorialScene(void);
	~TutorialScene(void)override;

private:

	// チュートリアルの描画UI
	enum class TUTORIAL_UI
	{
		NONE,
		MOUSE,
		KEY_WASD,
		KEY_R,
		KEY_Q,
		KEY_SPACE,
		KEY_ALL,

		X_BUTTON,
		A_BUTTON,
		B_BUTTON,
		D_PAD_BUTTON,
		CONTOROLLER_BUTTON,
	};

	bool isEndTutorial_;

	// 消去ボタン画像
	std::unique_ptr<ImageGUI>guiDeleteButton_;

	// チュートリアル画像
	std::vector<std::unique_ptr<Image>>imgTutorial_;
	std::vector<std::unique_ptr<Image>>imgTutorialExplain_;

	// チュートリアル映像
	std::vector<std::unique_ptr<Movie>>movieTutorial_;

	// チュートリアル関数
	std::vector<std::function<void(void)>>tutorialFunc_;

	// 数字画像
	std::unique_ptr<ImageDiv>imgNumbers_;
	// 実行回数の数字画像
	// imgNumbers_と画像が同じなので座標情報だけ読み込む
	Position2D numbersPos_;


	// 記号画像
	std::unique_ptr<ImageDiv>imgSymbols_;

	// 現在使用中のUI
	std::vector<TUTORIAL_UI>tutorialUi_;

	// チュートリアルのマウス画像
	std::unique_ptr<Image>imgMouse_;

	// WASDキー画像
	std::unique_ptr<Image>imgWASD_Key_;

	// Rキー画像
	std::unique_ptr<Image>imgR_Key_;

	// Qキー画像
	std::unique_ptr<Image>imgQ_Key_;

	// Spaceキー画像
	std::unique_ptr<Image>imgSpace_Key_;

	// Xボタン画像
	std::unique_ptr<Image>imgX_Button_;
	// Aボタン画像
	std::unique_ptr<Image>imgA_Button_;
	// Bボタン画像
	std::unique_ptr<Image>imgB_Button_;
	// 十字キー画像
	std::unique_ptr<Image>imgD_Pad_Button_;
	// コントローラー画像
	std::unique_ptr<Image>imgContoroller_;

	// すべてのキー画像
	std::unique_ptr<Image>imgAll_Key_;


	int maxTutorialStep_;

	int nowTutorialStep_;


	// 現在のチュートリアルの実行回数
	int tutorialCounter_;
	// 現在のチュートリアルの最大実行回数
	int tutorialMaxCounter_;

	void StartTutorial(void);

	void SetUpTutorial(LoadData::LoadDataInfo info, 
		ResourceManager::RESOURCE_ID imgId = ResourceManager::RESOURCE_ID::NONE,
		ResourceManager::RESOURCE_ID movieId = ResourceManager::RESOURCE_ID::NONE,
		ResourceManager::RESOURCE_ID imgExplainId = ResourceManager::RESOURCE_ID::NONE,
		TUTORIAL_UI uiId = TUTORIAL_UI::NONE,
		std::function<void(void)>func = nullptr);

	void InitAddSpecial(void)override;

	void UpdateAddSpecial(void)override;

	void SetUpActors(void)override;

	void InitBattleManager(void)override;

	void DrawAddSpecial(void)override;

	void DrawTutorialCounter(void);

	void DrawTutorialUi(void);

	// 現在のチュートリアルを次へ進める
	void NextStepTutorial(void);

	// チュートリアル用コールバック関数
	void TutorialCamera(void);
	void TutorialMove(void);
	void TutorialJump(void);
	void TutorialAvoid(void);
	void TutorialAttack(void);
	void TutorialDash(void);
	void TutorialCharge(void);
	void TutorialTechnique(void);
	void TutorialSpecial(void);
	void TutorialJustAvoidAttack(void);
	void TutorialSkill(void);
	void TutorialBattle(void);

};