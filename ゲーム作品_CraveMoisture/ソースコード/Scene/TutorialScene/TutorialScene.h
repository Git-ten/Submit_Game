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

	// �`���[�g���A���̕`��UI
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

	// �����{�^���摜
	std::unique_ptr<ImageGUI>guiDeleteButton_;

	// �`���[�g���A���摜
	std::vector<std::unique_ptr<Image>>imgTutorial_;
	std::vector<std::unique_ptr<Image>>imgTutorialExplain_;

	// �`���[�g���A���f��
	std::vector<std::unique_ptr<Movie>>movieTutorial_;

	// �`���[�g���A���֐�
	std::vector<std::function<void(void)>>tutorialFunc_;

	// �����摜
	std::unique_ptr<ImageDiv>imgNumbers_;
	// ���s�񐔂̐����摜
	// imgNumbers_�Ɖ摜�������Ȃ̂ō��W��񂾂��ǂݍ���
	Position2D numbersPos_;


	// �L���摜
	std::unique_ptr<ImageDiv>imgSymbols_;

	// ���ݎg�p����UI
	std::vector<TUTORIAL_UI>tutorialUi_;

	// �`���[�g���A���̃}�E�X�摜
	std::unique_ptr<Image>imgMouse_;

	// WASD�L�[�摜
	std::unique_ptr<Image>imgWASD_Key_;

	// R�L�[�摜
	std::unique_ptr<Image>imgR_Key_;

	// Q�L�[�摜
	std::unique_ptr<Image>imgQ_Key_;

	// Space�L�[�摜
	std::unique_ptr<Image>imgSpace_Key_;

	// X�{�^���摜
	std::unique_ptr<Image>imgX_Button_;
	// A�{�^���摜
	std::unique_ptr<Image>imgA_Button_;
	// B�{�^���摜
	std::unique_ptr<Image>imgB_Button_;
	// �\���L�[�摜
	std::unique_ptr<Image>imgD_Pad_Button_;
	// �R���g���[���[�摜
	std::unique_ptr<Image>imgContoroller_;

	// ���ׂẴL�[�摜
	std::unique_ptr<Image>imgAll_Key_;


	int maxTutorialStep_;

	int nowTutorialStep_;


	// ���݂̃`���[�g���A���̎��s��
	int tutorialCounter_;
	// ���݂̃`���[�g���A���̍ő���s��
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

	// ���݂̃`���[�g���A�������֐i�߂�
	void NextStepTutorial(void);

	// �`���[�g���A���p�R�[���o�b�N�֐�
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