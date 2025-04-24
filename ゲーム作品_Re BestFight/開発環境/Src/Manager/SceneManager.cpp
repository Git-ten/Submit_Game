#include <chrono>
#include <DxLib.h>
#include<EffekseerForDxLib.h>
#include"Event/Fade.h"
#include "../Scene/TitleScene.h"
#include"../Scene/GameScene.h"
#include"../Scene/SelectScene.h"
#include"../Scene/ResultScene.h"
#include"../Scene/DemoScene.h"
#include"../Scene/AboutGameScene.h"
#include"../Scene/Camera.h"
#include"../Application.h"
#include"ResourceManager.h"
#include"EffectManager.h"
#include"SoundManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//メインスクリーン
	Size2D size = Application::GetInstance().GetScreenSize();
	mainScreen_ = MakeScreen(size.x, size.y);

	//タイトルシーンID
	sceneId_ = SCENE_ID::TITLE;

	//シーンインスタンス
	scene_ = nullptr;

	fader_ = std::make_unique<Fade>();

	//シーンを変更中か否か
	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	//プレイヤー
	playerID_ = CHARA_ID::NONE;
	//敵
	enemyID_ = CHARA_ID::NONE;

	//CPUと戦うか
	cpuFlag_ = false;

	//バイブレーション回数
	vibrationCnt_ = 0;

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(250, 153, 153);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	//// ライトの設定
	SetUseLighting(true);

	// 真上から下に向かったライト
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 130000.0f);
}

void SceneManager::Update(void)
{
	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	if (!isSceneChanging_)
	{
		//シーンの更新
		scene_->Update();
	}
	else
	{
		//フェイド
		FadeManual();
	}
}

void SceneManager::Draw(void)
{
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)

	//裏画面に描画
	SetDrawScreen(mainScreen_);
	// 画面をクリア
	ClearDrawScreen();

	//描画
	scene_->Draw();
	fader_->Draw();

	DrawMainScreen();
}

void SceneManager::Destroy(void)
{
	scene_->Release();
	delete instance_;
}

void SceneManager::ChangeScene(const SCENE_ID& nextId)
{
	isSceneChanging_ = true;
	sceneId_ = nextId;
	fader_->FadeStart(Fade::FADE_STATE::FADE_OUT);
}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)const
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

void SceneManager::SetPlayerAndEnemy(CHARA_ID player, CHARA_ID enemy)
{
	//プレイヤー
	playerID_ = player;
	//敵
	enemyID_ = enemy;
}

void SceneManager::SetCpuFlag(bool flg)
{
	cpuFlag_ = flg;
}

bool SceneManager::GetCpuFlag(void) const
{
	return cpuFlag_;
}

int SceneManager::GetScreen(void) const
{
	return mainScreen_;
}

void SceneManager::SetVibrationCnt(int count)
{
	vibrationCnt_ = count;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
}

SceneManager::~SceneManager(void)
{
	DeleteGraph(mainScreen_);
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(const SCENE_ID& sceneId)
{
	// リソースの解放
	// 一旦、すべてのリソースを開放する
	if (sceneId != SCENE_ID::RESULT)
	{
		ResourceManager::GetInstance().Release();
		EffectManager::GetInstance().Release();
		SoundManager::GetInstance().Release();
	}
	if (scene_ != nullptr)
	{
		scene_->Release();
	}

	// シーンを変更する
	sceneId_ = sceneId;

	//変更先のシーンのインスタンスを生成
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		break;
	case SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>(playerID_, enemyID_);
		break;
	case SCENE_ID::SELECT:
		scene_ = std::make_unique<SelectScene>(cpuFlag_);
		break;
	case SCENE_ID::RESULT:
		scene_ = std::make_unique<ResultScene>(playerID_, enemyID_);
		break;
	case SCENE_ID::DEMO:
	{
		CHARA_ID chara_1 = CHARA_ID::NONE;
		CHARA_ID chara_2 = CHARA_ID::NONE;
		RandomChara(&chara_1, &chara_2);
		scene_ = std::make_unique<DemoScene>(chara_1, chara_2);
		break;
	}
	case SCENE_ID::ABOUT:
		scene_ = std::make_unique<AboutGameScene>(AboutGameScene::MODE::TITLE_ABOUT_GAME);
		break;
	default:
		break;
	}

	//シーンの初期化
	scene_->Init();

	//デルタタイムをセット
	ResetDeltaTime();
}

void SceneManager::FadeManual(void)
{
	fader_->Update();
	if (!fader_->IsFadeFlag() && !fader_->IsEndFlag())
	{
		fader_->FadeStart(Fade::FADE_STATE::FADE_IN);
		fader_->SetIsEndFlag(true);
		DoChangeScene(sceneId_);
	}
	else if (!fader_->IsFadeFlag() && fader_->IsEndFlag())
	{
		isSceneChanging_ = false;
	}
}

void SceneManager::RandomChara(CHARA_ID* chara_1, CHARA_ID* chara_2)
{
	std::vector<CHARA_ID>charaIds;
	charaIds.push_back(CHARA_ID::FIGHTER_ID);
	charaIds.push_back(CHARA_ID::SORDMAN_ID);
	charaIds.push_back(CHARA_ID::MAGICIAN_ID);


	int random = GetRand(2);

	SRand((unsigned int)time(NULL));

	*chara_1 = charaIds[random];
	charaIds.erase(charaIds.begin() + random);

	random = GetRand(1);
	*chara_2 = charaIds[random];

	charaIds.clear();
}

void SceneManager::DrawMainScreen(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	//描画対象スクリーンが切り替わりカメラがリセットされたため
	//カメラを強制更新
	if (scene_->GetCamera().lock() != nullptr)
	{
		scene_->GetCamera().lock()->CompCameraInfoUpdate();
	}
	DrawGraph(0, 0, mainScreen_, false);


	//バイブレーション処理
	if ((vibrationCnt_ % 2) != 0)
	{
		DrawGraph(4, 0, mainScreen_, false);
	}
	vibrationCnt_--;
	if (vibrationCnt_ < 0)vibrationCnt_ = 0;
}