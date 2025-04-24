#include <cassert>
#include "../../Application.h"
#include "../InputManager/InputManager.h"
#include "../EffectResourceManager/EffectResourceManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "../SoundResourceManager/SoundResourceManager.h"
#include "SceneManager.h"

#define DXLIB_LIGHT
#define BACKCULLING
#define FADE

#pragma region 定数宣言

const Dir3DF LIGHT_DIR = { 0.00f, -1.00f, 1.00f };

#pragma endregion

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance(void)
{
	assert(instance_ == nullptr || !"SceneManagerが作成されているのに再び生成しようとしています");

	instance_ = new SceneManager();
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	sceneStatus_ = std::make_unique<SceneStatus>();
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	// 真上から下に向かったライト
	lightDir_ = LIGHT_DIR;

#ifdef DXLIB_LIGHT
	// ライトの設定
	SetUseLighting(true);
	ChangeLightTypeDir(lightDir_);
#else
	// ライトの設定
	SetUseLighting(false);
#endif

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

#ifdef BACKCULLING
	// バックカリングを有効にする
	SetUseBackCulling(true);
#endif

#ifdef FADE
	fader_ = std::make_unique<Fader>();
#endif

	ChangeSceneState(SceneID::SCENE_ID::TITLE);
}

void SceneManager::Update(void)
{
	deltaTime_.UpdateTime();


#ifdef FADE
	// フェード中なら更新しない
	fader_->Update();

	if (fader_->IsScreenBrack() && fader_->IsFadeOut())sceneStatus_->InitScene();
	if (!fader_->IsFadeEnd())return;
#endif

	sceneStatus_->UpdateScene();
}

void SceneManager::Draw(void)
{

#ifdef FADE
	// フェードアウト時は描画しない
	if (!fader_->IsFadeOut())
	{
		camera_->DrawUpdate();
		sceneStatus_->DrawScene();
	}

	fader_->Draw();
#else
	camera_->DrawUpdate();
	sceneStatus_->DrawScene();
	sceneStatus_->DrawSceneSpecial();
#endif
	auto& input = InputManager::GetInstance();
	if (input.GetJPadNum() > 0)
	{
		if (GetMouseDispFlag())
		{
			SetMouseDispFlag(false);
		}
	}
}

void SceneManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

void SceneManager::LockSceneUpdate(void)
{
	camera_->LockIsCameraUpdate();
}

void SceneManager::UnLockSceneUpdate(void)
{
	camera_->UnLockIsCameraUpdate();
}

const bool SceneManager::IsLockSceneUpdate(void)const
{
	return camera_->IsLockCameraUpdate();
}

void SceneManager::ChangeSceneState(const SceneID::SCENE_ID& id)
{
	assert(sceneStatus_ != nullptr || !"SceneManager::ChangeSceneStateでシーンステータスがnullで参照しようとしました");
	
	// すべてのリソースを開放
	ResourceManager::GetInstance().Release();
	ResourceManager::GetInstance().Init();
	SoundResourceManager::GetInstance().Release();

	// カメラの初期化
	camera_->Init();

	Application::GetInstance().DefaultMouseDisp();

	sceneStatus_->ChangeSceneState(id);

	EffectResourceManager::GetInstance().Release();

#ifdef FADE
	FadeStart();
#endif

}

const bool SceneManager::IsEndGame(void)const
{
	return isEndGame_;
}

void SceneManager::EndGame(void)
{
	isEndGame_ = true;
}

const float SceneManager::GetDeltaTime(void)const
{
	return deltaTime_.GetNowTime();
}

const bool SceneManager::IsNullptr(void)
{
	return instance_ == nullptr;
}

const std::weak_ptr<Camera> SceneManager::GetCamera(void) const
{
	return camera_;
}

const Dir3DF& SceneManager::GetLightDir(void) const
{
	return lightDir_;
}

void SceneManager::SetUpSelectToGameScene(const SelectSceneToGameScene& data)
{
	toGameSceneData_ = data;
}

SceneManager::SelectSceneToGameScene SceneManager::GetSelectToGameSceneData(void)
{
	return toGameSceneData_;
}

void SceneManager::SetUpGameToResultScene(const GameSceneToResultScene& data)
{
	toResultSceneData_ = data;
}

SceneManager::GameSceneToResultScene SceneManager::GetGameToResultSceneData(void)
{
	return toResultSceneData_;
}

void SceneManager::FadeStart(void)
{
	fader_->FadeStart();
}

SceneManager::SceneManager(void):
	isEndGame_(false),
	deltaTime_(DeltaTime()),
	lightDir_(Dir3DF())
{
}