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
#include"AboutGameScene.h"
#include"../Manager/Event/Fade.h"
#include"../Application.h"
#include "GameScene.h"

//#define DEBUG_SHADE (1)
//#define DEBUG_FREE_CAMERA (2)

//コンストラクタ
GameScene::GameScene(SceneManager::CHARA_ID player, SceneManager::CHARA_ID enemy)
{
	poseFlag_ = false;
	//プレイヤーID
	playerId_ = player;
	//敵ID
	enemyId_ = enemy;

	nowSceneStat_ = SCENE_STATE::START;

	aboutFlag_ = false;
	countDown_ = 0.0f;
	endTime_ = 0.0f;
	prevCountDown_ = 0.0f;
	remainTime_ = 0.0f;
	strExRate_ = 1.0f;
	imgExplainEx_ = -1;
	startFlag_ = false;
}

//デストラクタ
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//ゲーム説明画像
	imgExplainEx_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EXPLAIN_EX).lock()->handleId_;
	fader_ = std::make_unique<Fade>();

	startFlag_ = false;

	poseFlag_ = false;
	pose_ = std::make_unique<StopScene>();
	pose_->Init();

	camera_ = std::make_shared<Camera>();
	camera_->Init();

	//デバッグ用
	enemy_ = std::make_shared<Player>(camera_, SceneManager::GetInstance().GetCpuFlag());
	player_ = std::make_shared<Player>(camera_, false);
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

	//印
	enemy_->InitMark();
	player_->InitMark();

	enemy_->charactor_->SetActorCamera(enemy_->actorCam_);
	player_->charactor_->SetActorCamera(player_->actorCam_);
	

	skyDome_ = std::make_unique<SkyDome>(player_, enemy_);
	skyDome_->Init();

	stage_ = std::make_unique<Stage>(player_, enemy_, camera_);
	stage_->Init();

	//決定音
	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION).lock();
	//勝負あり音
	finishSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::FINISH).lock();
	//カウントダウン音
	CountDownSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::COUNT_DOWN).lock();
	//スタート音
	StartSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::START).lock();


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

	//カメラをシンクロ
	//カメラをシンクロ(適当な回数だけ更新)
	int cameraCnt = 102;
	for (int ii = 0; ii < cameraCnt; ii++)
	{
		EffectManager::GetInstance().Update();
		player_->actorCam_->DrawUpdate();
	}
	SynchroCamera();

	ChangeState(SCENE_STATE::START);

	aboutFlag_ = false;
	aboutGameScene_ = std::make_unique<AboutGameScene>(AboutGameScene::MODE::GAME_STOPMENYU);
	aboutGameScene_->Init();

	gameSound_ = SoundManager::GetInstance().Load(SoundManager::SOD::BATTLE_BGM);
}

void GameScene::Update()
{
	update_();
}

void GameScene::Draw()
{
	draw_();
}

void GameScene::Release()
{
	
}

void GameScene::ChangeState(const SCENE_STATE& stat)
{
	nowSceneStat_ = stat;

	switch (nowSceneStat_)
	{
	case SCENE_STATE::START:
		//スタート演出の時のカウントダウン
		countDown_ = COUNT_DOWN_MAX;
		prevCountDown_ = COUNT_DOWN_MAX;
		goEff_ = EffectManager::GetInstance().Load(EffectManager::EFF::FIRE_CIRCLE);
		//文字の拡大率
		strExRate_ = EX_DEFAULT_RATE;
		if (!Application::GetInstance().IsWidowMode())strExRate_ *= 2;
		update_ = [&]() {UpdateStart();};
		draw_ = [&]() {DrawStart();};
		break;
	case SCENE_STATE::PLAY:
		update_ = [&]() {UpdatePlay();};
		draw_ = [&]() {DrawPlay();};
		break;
	case SCENE_STATE::END:
		strExRate_ = 0.0f;
		endTime_ = END_MAX_TIME;
		update_ = [&]() {UpdateEnd();};
		draw_ = [&]() {DrawEnd();};
		break;
	default:
		break;
	}
}

void GameScene::SynchroCamera(void)
{
	if (player_->GetState() == ActorBase::STATE::UNLEASH_SKILL)
	{
		camera_->SetSpeed(200.0f);
		camera_->CompSetCameraInfo(player_->actorCam_->GetPos(), player_->actorCam_->GetTarget(), player_->actorCam_->GetRot());
	}
	else if (enemy_->GetState() == ActorBase::STATE::UNLEASH_SKILL)
	{
		camera_->SetSpeed(200.0f);
		camera_->CompSetCameraInfo(enemy_->actorCam_->GetPos(), enemy_->actorCam_->GetTarget(), enemy_->actorCam_->GetRot());
	}
	else if (player_->actorCam_->GetCameraMode() == Camera::MODE::SPECIAL_ATTACK_MODE ||
		player_->actorCam_->GetCameraMode() == Camera::MODE::JUST)
	{
		camera_->SetSpeed(200.0f);
		camera_->CompSetCameraInfo(player_->actorCam_->GetPos(), player_->actorCam_->GetTarget(), player_->actorCam_->GetRot());
	}
	else if (enemy_->actorCam_->GetCameraMode() == Camera::MODE::SPECIAL_ATTACK_MODE ||
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

void GameScene::DrawLimitTime(void)
{
	int minute = MyUtility::Second2Minute(remainTime_);
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	int fontSize = 32;

	if (!Application::GetInstance().IsWidowMode())fontSize = 64;

	SetFontSize(fontSize);
	DrawFormatString(screenSize.x / 2 - fontSize, 0, 0xff0000, "%0d分%0d秒", minute, static_cast<int>(remainTime_) % 60);
	SetFontSize(16);
}

void GameScene::SubRemainTime(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	remainTime_ -= delta;
}

void GameScene::UpdateStart(void)
{
	fader_->Update();

	if (!startFlag_)
	{
		InputManager& ins = InputManager::GetInstance();

		if (Application::GetInstance().IsMouseFlag())
		{
			if (ins.IsTrgUp(KEY_INPUT_RETURN))
			{
				startFlag_ = true;
				fader_->FadeStart();
				gameSound_.lock()->Play(DX_PLAYTYPE_LOOP);
			}
		}
		else
		{
			if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				startFlag_ = true;
				fader_->FadeStart();
				gameSound_.lock()->Play(DX_PLAYTYPE_LOOP);
			}
		}
	}

	else if (!fader_->IsFadeFlag())
	{
		if (!poseFlag_)
		{
			//デバッグ用
			//enemy_->Update();
			enemy_->animController_->Update();

			//player_->Update();
			player_->animController_->Update();

			SynchroCamera();

			stage_->Update();

			EffectManager::GetInstance().Update();

			InputManager& ins = InputManager::GetInstance();

			//ポーズ処理
			if (ins.IsTrgUp(KEY_INPUT_ESCAPE) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::MENYU_TRIGGER) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::MENYU_TRIGGER))
			{
				poseFlag_ = true;
				pose_->Init();
			}

			//カウントダウン
			float delta = SceneManager::GetInstance().GetDeltaTime();
			countDown_ -= delta;

			float speed = EX_RATE_SPEED;
			float stop = prevCountDown_ - 0.9f;
			if (!Application::GetInstance().IsWidowMode())speed *= 1.5f;

			strExRate_ -= speed;

			if (countDown_ < stop)
			{
				prevCountDown_--;
				countDown_ = prevCountDown_;
				strExRate_ = EX_DEFAULT_RATE;
				if (!Application::GetInstance().IsWidowMode())strExRate_ *= 2.0f;
				//カウントダウン音を鳴らす
				CountDownSod_.lock()->Play(DX_PLAYTYPE_BACK);
			}
			if (countDown_ < 1.0f)
			{
				float scl = 100.0f;
				if (!Application::GetInstance().IsWidowMode())scl = 150.0f;
				strExRate_ = 0.0f;
				goEff_.lock()->Play();
				goEff_.lock()->SetRot({ 90.0f, 90.0f, 0.0f });
				goEff_.lock()->SetScl({ scl, scl, scl });
				Size2D screenSize = Application::GetInstance().GetScreenSize();
				goEff_.lock()->SetPos({ static_cast<float>(screenSize.x / 2), static_cast<float>(screenSize.y / 2), 0.0f });
				ChangeState(SCENE_STATE::PLAY);
				//スタート音を鳴らす
				StartSod_.lock()->Play(DX_PLAYTYPE_BACK);
			}
		}
		else if (aboutFlag_)
		{
			aboutGameScene_->Update();

			if (aboutGameScene_->IsEnd())
			{
				aboutFlag_ = false;
				pose_->SetDecision(StopScene::STATE::SELECT_MENYU);
			}
		}
		else
		{
			pose_->Update();

			StopScene::STATE decision = pose_->GetDecision();
			switch (decision)
			{
			case StopScene::STATE::RETURN_SELECT:
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
				//SceneManager::GetInstance().SetCpuFlag(true);
				gameSound_.lock()->Stop();
				break;
			case StopScene::STATE::RETURN_MENYU:
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				poseFlag_ = false;
				break;
			case StopScene::STATE::ABOUT_GAME:
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				aboutFlag_ = true;
				aboutGameScene_->OpenAboutGameScene();
				//poseFlag_ = false;
				break;
			default:
				break;
			}
		}
	}
}

void GameScene::DrawStart(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();

	if (startFlag_ && fader_->GetNowState() != Fade::FADE_STATE::FADE_OUT)
	{
		if (aboutFlag_)
		{
			// クリップ距離を設定する(SetDrawScreenでリセットされる)
			SetCameraNearFar(Camera::CAMERA_NEAR, Camera::CAMERA_FAR);
			Position3DF pos = { -500.0f, 240.0f, -1260.0f };
			camera_->SetPos(pos);
			camera_->SetTarget({ 320.0f, 240.0f, 1.0f });
			camera_->CompCameraInfoUpdate();
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			Size2D screenSize = Application::GetInstance().GetScreenSize();
			DrawBox(0, 0, screenSize.x - 1, screenSize.y - 1, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			aboutGameScene_->Draw();
		}
		else
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

			int fontSize = 16;
			int stop = 64;
			if (!Application::GetInstance().IsWidowMode())stop = 96;

			//カウントダウン
			if (strExRate_ < stop)
			{
				SetFontSize(stop);
				fontSize = stop;
			}
			else
			{
				SetFontSize(INT_CAST(strExRate_));
				fontSize = INT_CAST(strExRate_);
			}
			DrawFormatString(screenSize.x / 2 - fontSize / 5, 128 - fontSize / 2, 0x000000, "%d", static_cast<int>(countDown_));
			DrawFormatString(screenSize.x / 2 - fontSize / 5 + 3, 128 - fontSize / 2 + 3, 0xdddddd, "%d", static_cast<int>(countDown_));
			SetFontSize(16);

			if (poseFlag_)
			{
				pose_->Draw();
			}
		}
	}
	else
	{
		static int strCnt = 0;
		int fontSize = 50;
		int shiftX = 520;
		DrawExtendGraph(0, 0, screenSize.x, screenSize.y, imgExplainEx_, true);
		if (!Application::GetInstance().IsWidowMode())
		{
			shiftX = 970;
			fontSize = 90;
		}

		SetFontSize(fontSize);
		if (strCnt % 20)
		{
			DrawFormatString(screenSize.x - shiftX, fontSize + 10, 0xff0000, "Start By Enter or A");
		}
		else
		{
			DrawFormatString(screenSize.x - shiftX, fontSize + 10, 0xffffff, "Start By Enter or A");
		}
		strCnt++;
		if (strCnt > 300)strCnt = 0;
		SetFontSize(16);
	}

	fader_->Draw();
}

void GameScene::UpdatePlay(void)
{
	if (!poseFlag_)
	{
		//デバッグ用
		enemy_->Update();

		player_->Update();

		SynchroCamera();

		skyDome_->Update();
		stage_->Update();

		EffectManager::GetInstance().Update();

		SubRemainTime();

		InputManager& ins = InputManager::GetInstance();

		//ポーズ処理
		if (ins.IsTrgUp(KEY_INPUT_ESCAPE) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::MENYU_TRIGGER) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::MENYU_TRIGGER))
		{
			poseFlag_ = true;
			pose_->Init();
		}


		//GO文字の拡大率
		float stop = EX_DEFAULT_RATE;
		if (!Application::GetInstance().IsWidowMode())stop *= 2.0f;

		if (strExRate_ < stop)
		{
			float speed = EX_RATE_SPEED;
			if (!Application::GetInstance().IsWidowMode())speed *= 2.0f;

			strExRate_ += speed;
		}
	}
	else if(aboutFlag_)
	{
		aboutGameScene_->Update();

		if (aboutGameScene_->IsEnd())
		{
			aboutFlag_ = false;
			pose_->SetDecision(StopScene::STATE::SELECT_MENYU);
		}
	}
	else
	{
		pose_->Update();

		StopScene::STATE decision = pose_->GetDecision();
		switch (decision)
		{
		case StopScene::STATE::RETURN_SELECT:
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
			//SceneManager::GetInstance().SetCpuFlag(true);
			gameSound_.lock()->Stop();
			break;
		case StopScene::STATE::RETURN_MENYU:
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			poseFlag_ = false;
			break;
		case StopScene::STATE::ABOUT_GAME:
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			aboutFlag_ = true;
			aboutGameScene_->OpenAboutGameScene();
			//poseFlag_ = false;
			break;
		default:
			break;
		}
	}


	float fSpeed = 30.0f;
	//勝利条件：片方のHPが0になる
	if (player_->userInterface_->GetStateHp() <= 0.0f &&
		enemy_->GetState() != ActorBase::STATE::UNLEASH_SKILL)
	{
		finishSod_.lock()->Play(DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().SetPlayerAndEnemy(enemy_->GetModelResource(), player_->GetModelResource());
		ChangeState(SCENE_STATE::END);
		goEff_.lock()->Play();
		goEff_.lock()->SetRot({ 90.0f, 90.0f, 0.0f });
		float scl = 200.0f;
		if (!Application::GetInstance().IsWidowMode())scl = 300.0f;
		goEff_.lock()->SetScl({ scl, scl, scl });
		Size2D screenSize = Application::GetInstance().GetScreenSize();
		goEff_.lock()->SetPos({ static_cast<float>(screenSize.x / 2), static_cast<float>(screenSize.y / 2), 0.0f});
		enemy_->SetStopFrame(fSpeed);
		player_->SetStopFrame(fSpeed);
		//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}
	else if (enemy_->userInterface_->GetStateHp() <= 0.0f &&
		player_->GetState() != ActorBase::STATE::UNLEASH_SKILL)
	{
		finishSod_.lock()->Play(DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().SetPlayerAndEnemy(player_->GetModelResource(), enemy_->GetModelResource());
		ChangeState(SCENE_STATE::END);
		goEff_.lock()->Play();
		goEff_.lock()->SetRot({ 90.0f, 90.0f, 0.0f });
		float scl = 200.0f;
		if (!Application::GetInstance().IsWidowMode())scl = 300.0f;
		goEff_.lock()->SetScl({ scl, scl, scl });
		Size2D screenSize = Application::GetInstance().GetScreenSize();
		goEff_.lock()->SetPos({ static_cast<float>(screenSize.x / 2), static_cast<float>(screenSize.y / 2), 0.0f });
		enemy_->SetStopFrame(fSpeed);
		player_->SetStopFrame(fSpeed);
		//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	//勝負条件:時間切れ
	if (remainTime_ <= 0.0f)
	{
		if (player_->userInterface_->GetStateHp() >= enemy_->userInterface_->GetStateHp() &&
			enemy_->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			finishSod_.lock()->Play(DX_PLAYTYPE_BACK);
			SceneManager::GetInstance().SetPlayerAndEnemy(player_->GetModelResource(), enemy_->GetModelResource());
			ChangeState(SCENE_STATE::END);
			goEff_.lock()->Play();
			goEff_.lock()->SetRot({ 90.0f, 90.0f, 0.0f });
			float scl = 200.0f;
			if (!Application::GetInstance().IsWidowMode())scl = 300.0f;
			goEff_.lock()->SetScl({ scl, scl, scl });
			Size2D screenSize = Application::GetInstance().GetScreenSize();
			goEff_.lock()->SetPos({ static_cast<float>(screenSize.x / 2), static_cast<float>(screenSize.y / 2), 0.0f });
			enemy_->SetStopFrame(fSpeed);
			player_->SetStopFrame(fSpeed);
			//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		}
		else if (player_->userInterface_->GetStateHp() < enemy_->userInterface_->GetStateHp() &&
			player_->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			finishSod_.lock()->Play(DX_PLAYTYPE_BACK);
			SceneManager::GetInstance().SetPlayerAndEnemy(enemy_->GetModelResource(), player_->GetModelResource());
			ChangeState(SCENE_STATE::END);
			goEff_.lock()->Play();
			goEff_.lock()->SetRot({ 90.0f, 90.0f, 0.0f });
			float scl = 200.0f;
			if (!Application::GetInstance().IsWidowMode())scl = 300.0f;
			goEff_.lock()->SetScl({ scl, scl, scl });
			Size2D screenSize = Application::GetInstance().GetScreenSize();
			goEff_.lock()->SetPos({ static_cast<float>(screenSize.x / 2), static_cast<float>(screenSize.y / 2), 0.0f });
			enemy_->SetStopFrame(fSpeed);
			player_->SetStopFrame(fSpeed);
			//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		}
	}
}

void GameScene::DrawPlay(void)
{
	if (aboutFlag_)
	{
		// クリップ距離を設定する(SetDrawScreenでリセットされる)
		SetCameraNearFar(Camera::CAMERA_NEAR, Camera::CAMERA_FAR);
		Position3DF pos = { -500.0f, 240.0f, -1260.0f };
		camera_->SetPos(pos);
		camera_->SetTarget({ 320.0f, 240.0f, 1.0f });
		camera_->CompCameraInfoUpdate();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		Size2D screenSize = Application::GetInstance().GetScreenSize();
		DrawBox(0, 0, screenSize.x - 1, screenSize.y - 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		aboutGameScene_->Draw();
	}
	else
	{
		camera_->DrawUpdate();

		skyDome_->Draw();
		stage_->Draw();

		EffectManager::GetInstance().Draw3DEffect();

		player_->Draw();
		enemy_->Draw();

		DrawLimitTime();

		Size2D screenSize = Application::GetInstance().GetScreenSize();
		float stop = EX_DEFAULT_RATE;
		if (!Application::GetInstance().IsWidowMode())stop *= 2.0f;

		if (strExRate_ < stop)
		{
			int fontSize = 16;
			SetFontSize(INT_CAST(strExRate_));
			fontSize = INT_CAST(strExRate_);
			DrawFormatString(screenSize.x / 2 - fontSize / 2, screenSize.y / 2 - fontSize / 2, 0x000000, "Go");
			SetFontSize(16);
		}


		EffectManager::GetInstance().Draw2DEffect();
		if (poseFlag_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			Size2D screenSize = Application::GetInstance().GetScreenSize();
			DrawBox(0, 0, screenSize.x - 1, screenSize.y - 1, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			pose_->Draw();
		}
	}
}

void GameScene::UpdateEnd(void)
{
	enemy_->Update();

	player_->Update();

	SynchroCamera();

	stage_->Update();

	EffectManager::GetInstance().Update();

	float delta = SceneManager::GetInstance().GetDeltaTime();
	endTime_ -= delta;
	strExRate_ += EX_RATE_SPEED;

	float stop = FINISH_STR_RATE;
	if (!Application::GetInstance().IsWidowMode())stop *= 2.0f;

	if (strExRate_ > stop)
	{
		strExRate_ = stop;
	}
	if (endTime_ < 0.0f)
	{
		gameSound_.lock()->Stop();
		player_->StopAllEffect();
		enemy_->StopAllEffect();
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}
}

void GameScene::DrawEnd(void)
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

	Size2D screenSize = Application::GetInstance().GetScreenSize();
	int fontSize = 16;
	SetFontSize(INT_CAST(strExRate_));
	fontSize = INT_CAST(strExRate_);
	if (remainTime_ > 0.0f)
	{
		fontSize += 45;
		DrawFormatString(screenSize.x / 2 - fontSize, screenSize.y / 2 - fontSize / 3, 0x000000, "FINISH");
		DrawFormatString(screenSize.x / 2 - fontSize + 10, screenSize.y / 2 - fontSize / 3 + 10, 0xffdddd, "FINISH");
	}
	else
	{
		fontSize += 45;
		DrawFormatString(screenSize.x / 2 - fontSize, screenSize.y / 2 - fontSize / 3, 0x000000, "TIME UP");
		DrawFormatString(screenSize.x / 2 - fontSize + 10, screenSize.y / 2 - fontSize / 3 + 10, 0xffdddd, "TIME UP");
	}
	SetFontSize(16);
}