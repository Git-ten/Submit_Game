#include"Act/Player.h"
#include"Act/Stage.h"
#include"Act/SkyDome.h"
#include"Act/CharactorData/CharactorBase.h"
#include"Act/Common/AnimationController.h"
#include"Camera.h"
#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include"StopScene.h"
#include"../Application.h"
#include "DemoScene.h"

//#define DEBUG_SHADE (1)
//#define DEBUG_FREE_CAMERA (2)

//コンストラクタ
DemoScene::DemoScene(SceneManager::CHARA_ID player, SceneManager::CHARA_ID enemy)
{
	//プレイヤーID
	playerId_ = player;
	//敵ID
	enemyId_ = enemy;
}

//デストラクタ
DemoScene::~DemoScene()
{
}

void DemoScene::Init()
{
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	//デバッグ用
	enemy_ = std::make_shared<Player>(camera_, true);
	player_ = std::make_shared<Player>(camera_, true);
	//初期化を一緒にする際に互いのアドレスを参照している
	enemy_->Init(player_, enemyId_);
	player_->Init(enemy_, playerId_);

	//敵は所定の場所に移動しておく
	enemy_->transform_->pos = VGet(0.0f, 0.0f, 1800.0f);
	enemy_->transform_->prevPos = enemy_->transform_->pos;
	enemy_->transform_->Update();
	enemy_->userInterface_->SetMirrorFlag(true);

	enemy_->InitCamera();
	player_->InitCamera();
	enemy_->charactor_->SetActorCamera(enemy_->actorCam_);
	player_->charactor_->SetActorCamera(player_->actorCam_);


	stage_ = std::make_unique<Stage>(player_, enemy_, camera_);
	stage_->Init();

	skyDome_ = std::make_unique<SkyDome>(player_, enemy_);
	skyDome_->Init();

	gameSound_ = SoundManager::GetInstance().Load(SoundManager::SOD::BATTLE_BGM);
	gameSound_.lock()->Play(DX_PLAYTYPE_LOOP);

	remainTime_ = LIMIT_TIME;

	//カメラの設定
#ifndef DEBUG_FREE_CAMERA
	camera_->ChangeMode(Camera::MODE::SYNCHRO);
	camera_->SetDefaultMode(Camera::MODE::SYNCHRO);
	//camera_->SetFollower(player_->transform_, enemy_->transform_);
#endif
#ifdef DEBUG_FREE_CAMERA
	camera_->ChangeMode(Camera::MODE::FREE);
	camera_->SetDefaultMode(Camera::MODE::FREE);
	camera_->SetCameraTarget(player_->transform_);
#endif

	//カメラをシンクロ(適当な回数だけ更新)
	int cameraCnt = 102;
	for (int ii = 0; ii < cameraCnt; ii++)player_->actorCam_->DrawUpdate();
	SynchroCamera();
}

void DemoScene::Update()
{
	//デバッグ用
	enemy_->Update();

	player_->Update();

	SynchroCamera();

	stage_->Update();

	EffectManager::GetInstance().Update();

	SubRemainTime();

	//勝利条件：片方のHPが0になる
	if (player_->userInterface_->GetStateHp() <= 0.0f)
	{
		gameSound_.lock()->Stop();
		player_->StopAllEffect();
		enemy_->StopAllEffect();
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	else if (enemy_->userInterface_->GetStateHp() <= 0.0f)
	{
		gameSound_.lock()->Stop();
		player_->StopAllEffect();
		enemy_->StopAllEffect();
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//勝負条件:時間切れ
	if (remainTime_ <= 0.0f)
	{
		gameSound_.lock()->Stop();
		player_->StopAllEffect();
		enemy_->StopAllEffect();
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//ボタンを押したらデモプレイ終了
	if (CheckHitKeyAll() || GetJoypadInputState(DX_INPUT_PAD1) ||
		GetJoypadInputState(DX_INPUT_PAD2))
	{
		gameSound_.lock()->Stop();
		player_->StopAllEffect();
		enemy_->StopAllEffect();
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void DemoScene::Draw()
{
	camera_->DrawUpdate();

	skyDome_->Draw();
	stage_->Draw();

	EffectManager::GetInstance().Draw3DEffect();

	player_->Draw();

	//デバッグ用
	enemy_->Draw();

	DrawLimitTime();

	EffectManager::GetInstance().Draw2DEffect();
}

void DemoScene::Release()
{

}

void DemoScene::SynchroCamera(void)
{
	if (player_->GetState() == ActorBase::STATE::UNLEASH_SKILL ||
		player_->actorCam_->GetCameraMode() == Camera::MODE::SPECIAL_ATTACK_MODE ||
		player_->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		camera_->SetSpeed(200.0f);
		camera_->CompSetCameraInfo(player_->actorCam_->GetPos(), player_->actorCam_->GetTarget(), player_->actorCam_->GetRot());
	}
	else if (enemy_->GetState() == ActorBase::STATE::UNLEASH_SKILL ||
		enemy_->actorCam_->GetCameraMode() == Camera::MODE::SPECIAL_ATTACK_MODE ||
		enemy_->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		camera_->SetSpeed(200.0f);
		camera_->CompSetCameraInfo(enemy_->actorCam_->GetPos(), enemy_->actorCam_->GetTarget(), enemy_->actorCam_->GetRot());
	}
	else
	{
		float PDis = VSquareSize(VSub(player_->transform_->pos, camera_->GetPos()));
		float EDis = VSquareSize(VSub(enemy_->transform_->pos, camera_->GetPos()));

		if (PDis <= EDis)
		{
			camera_->CompSetCameraInfo(player_->actorCam_->GetPos(), player_->actorCam_->GetTarget(), player_->actorCam_->GetRot());
		}
		else
		{
			camera_->CompSetCameraInfo(enemy_->actorCam_->GetPos(), enemy_->actorCam_->GetTarget(), enemy_->actorCam_->GetRot());
		}

		camera_->SetDefaultSpeed();
	}
}

void DemoScene::DrawLimitTime(void)
{
	int minute = MyUtility::Second2Minute(remainTime_);
	Size2D screenSize = Application::GetInstance().GetScreenSize();

	SetFontSize(32);
	DrawFormatString(screenSize.x / 2 - 32, 0, 0xff0000, "%0d分%0d秒", minute, static_cast<int>(remainTime_) % 60);
	SetFontSize(16);
}

void DemoScene::SubRemainTime(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	remainTime_ -= delta;
}