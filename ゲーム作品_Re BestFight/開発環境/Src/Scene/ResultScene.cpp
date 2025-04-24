#include<DxLib.h>
#include"../Utility/VectorHeader.h"
#include"../Application.h"
#include"Act/Common/Transform.h"
#include"Act/Common/AnimationController.h"
#include"Camera.h"
#include"ResultMenyu.h"
#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include "ResultScene.h"

ResultScene::ResultScene(SceneManager::CHARA_ID winner, SceneManager::CHARA_ID loser)
{
	imgResultHandle_ = -1;

	//����
	winnerId_ = winner;
		/*std::make_unique<Transform>();
	winner_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(winner));*/
	//�s��
	loserId_ = loser;
		/*std::make_unique<Transform>();
	loser_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(loser));*/

	nowState_ = SCENE_STATE::RESULT;
}

//�f�X�g���N�^
ResultScene::~ResultScene()
{

}

void ResultScene::Init()
{
	//���U���g�摜�̃n���h���ԍ����i�[
	imgResultHandle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RESULT).lock()->handleId_;

	InitCharactor();

	winner_->pos = {-400.0f, 0.0f, 450.0f};
	winner_->scl = { 2.0f, 2.0f, 2.0f };
	winner_->quaRotLocal = Quaternion();
	winner_->Update();
	winnerAnim_ = std::make_unique<AnimationController>(winner_->modelId);
	winnerAnim_->ClearAnimMap();

	loser_->pos = {400.0f, -300.0f, 300.0f};
	loser_->scl = { 2.0f, 2.0f, 2.0f };
	loser_->quaRotLocal = Quaternion();
	loser_->Update();
	loserAnim_ = std::make_unique<AnimationController>(loser_->modelId);
	loserAnim_->ClearAnimMap();

	InitCharaAnim();

	camera_ = std::make_unique<Camera>();
	//�J�����̐ݒ�
	camera_->ChangeMode(Camera::MODE::NONE);
	camera_->SetDefault();

	//���艹
	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION).lock();
	//���U���g��
	resultSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::VICTORY).lock();
	resultSod_.lock()->Play(DX_PLAYTYPE_BACK);

	//���j���[�V�[��
	menyu_ = std::make_unique<ResultMenyu>();

	InitShader();

	ChangeState(SCENE_STATE::RESULT);
}

void ResultScene::Update()
{
	update_();
}

void ResultScene::Draw()
{
	draw_();
}

void ResultScene::Release()
{

}

void ResultScene::UpdateResult(void)
{
	InputManager& ins = InputManager::GetInstance();

	bool decision = false;
	if (Application::GetInstance().IsMouseFlag())
	{
		decision = ins.IsTrgUpMouseLeft() ||
				CheckHitKeyAll() != 0;
	}
	else
	{
		decision = ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN);
	}

	//�}�E�X�Ő�ɐi��
	if (decision)
	{
		ChangeState(SCENE_STATE::MENYU);
		//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	winnerAnim_->Update();
	loserAnim_->Update();
}


void ResultScene::DrawResultScene()
{
	camera_->DrawUpdate();

	//���U���g�摜��`��
	DrawResult();
	MV1DrawModel(winner_->modelId);
	MV1DrawModel(loser_->modelId);
}

//���U���g�摜��`��
void ResultScene::DrawResult()
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	DrawExtendGraph(0, 0, screenSize.x, screenSize.y, imgResultHandle_, true);
}

void ResultScene::UpdateMenyu(void)
{
	winnerAnim_->Update();
	loserAnim_->Update();

	//���j���[�̕`��
	menyu_->Update();

	ResultMenyu::STATE decision = menyu_->GetDecision();
	switch (decision)
	{
	case ResultMenyu::STATE::RETURN_TITLE:
		decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		break;
	case ResultMenyu::STATE::RETURN_SELECT:
		decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		break;
	default:
		break;
	}
}

void ResultScene::DrawMenyu(void)
{
	camera_->DrawUpdate();
	//���U���g�摜��`��
	DrawResult();
	MV1DrawModel(winner_->modelId);
	MV1DrawModel(loser_->modelId);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	DrawBox(0, 0, screenSize.x - 1, screenSize.y - 1, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//���j���[�̕`��
	menyu_->Draw();
}

void ResultScene::InitCharactor()
{
	switch (winnerId_)
	{
	case::SceneManager::CHARA_ID::FIGHTER_ID:
		winner_ = std::make_unique<Transform>();
		winner_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIST));
		break;
	case::SceneManager::CHARA_ID::SORDMAN_ID:
		winner_ = std::make_unique<Transform>();
		winner_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_SORD));
		break;
	case::SceneManager::CHARA_ID::MAGICIAN_ID:
		winner_ = std::make_unique<Transform>();
		winner_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGIC));
		break;
	default:
		break;
	}

	switch (loserId_)
	{
	case::SceneManager::CHARA_ID::FIGHTER_ID:
		loser_ = std::make_unique<Transform>();
		loser_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIST));
		break;
	case::SceneManager::CHARA_ID::SORDMAN_ID:
		loser_ = std::make_unique<Transform>();
		loser_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_SORD));
		break;
	case::SceneManager::CHARA_ID::MAGICIAN_ID:
		loser_ = std::make_unique<Transform>();
		loser_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGIC));
		break;
	default:
		break;
	}
}

void ResultScene::InitCharaAnim(void)
{
	std::string PATH_ANIM = "";

	switch (winnerId_)
	{
	case::SceneManager::CHARA_ID::FIGHTER_ID:
		PATH_ANIM = "Data/Model/Player/Fist/";
		winnerAnim_->Add(0, PATH_ANIM + "Win.mv1", 60.0f, 0.0f, 60.0f, true);
		winnerAnim_->Play(0);
		break;
	case::SceneManager::CHARA_ID::SORDMAN_ID:
		PATH_ANIM = "Data/Model/Player/Sord/";
		winnerAnim_->Add(0, PATH_ANIM + "Win_��.mv1", 20.0f, 0.0f, 30.0f, true);
		winnerAnim_->Play(0);
		break;
	case::SceneManager::CHARA_ID::MAGICIAN_ID:
		PATH_ANIM = "Data/Model/Player/Magic/";
		winnerAnim_->Add(0, PATH_ANIM + "Win_��.mv1", 20.0f, 0.0f, 35.0f, true);
		winnerAnim_->Play(0);
		break;
	default:
		break;
	}

	switch (loserId_)
	{
	case::SceneManager::CHARA_ID::FIGHTER_ID:
		PATH_ANIM = "Data/Model/Player/Fist/";
		loserAnim_->Add(0, PATH_ANIM + "Lose.mv1", 60.0f, 0.0f, 100.0f, true);
		loserAnim_->Play(0);
		break;
	case::SceneManager::CHARA_ID::SORDMAN_ID:
		PATH_ANIM = "Data/Model/Player/Sord/";
		loserAnim_->Add(0, PATH_ANIM + "Lose_��.mv1", 20.0f, 0.0f, 40.0f, true);
		loserAnim_->Play(0);
		break;
	case::SceneManager::CHARA_ID::MAGICIAN_ID:
		PATH_ANIM = "Data/Model/Player/Magic/";
		loserAnim_->Add(0, PATH_ANIM + "Lose_��.mv1", 40.0f, 0.0f, 60.0f, true);
		loserAnim_->Play(0);
		break;
	default:
		break;
	}
}

void ResultScene::ChangeState(const SCENE_STATE& stat)
{
	nowState_ = stat;

	switch (nowState_)
	{
	case SCENE_STATE::RESULT:
		update_ = [&]() {UpdateResult(); };
		draw_ = [&]() {DrawResultScene(); };
		break;
	case SCENE_STATE::MENYU:
		menyu_->Init();
		update_ = [&]() {UpdateMenyu(); };
		draw_ = [&]() {DrawMenyu(); };
		break;
	default:
		break;
	}
}