#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include"Manager/EffectManager.h"
#include"Manager/SoundManager.h"
#include"Debuger.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

//#define WINDOW_MODE (1)

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	//ゲーム終了フラグ
	isGameEnd_ = false;

	// アプリケーションの初期設定
	SetWindowText("Re:Best Fight");

	//設定したウィンドウモードを設定
	//true = ウィンドウモード,false = フルスクリーンモード
#ifdef WINDOW_MODE
	windowMode_ = true;
#else
	windowMode_ = false;
#endif


	if (windowMode_)
	{
		// ウィンドウサイズ
		SetGraphMode(WINDOW_SCREEN_SIZE_X, WINDOW_SCREEN_SIZE_Y, 32);
		screenSize_ = { WINDOW_SCREEN_SIZE_X , WINDOW_SCREEN_SIZE_Y };
		//マップの大きさを格納
		mapSize_ = {MAP_WSIZE_X, MAP_WSIZE_Y};
	}
	else
	{
		// ウィンドウサイズ
		SetGraphMode(FULL_SCREEN_SIZE_X, FULL_SCREEN_SIZE_Y, 32);
		screenSize_ = { FULL_SCREEN_SIZE_X , FULL_SCREEN_SIZE_Y };
		//マップの大きさを格納
		mapSize_ = { MAP_FSIZE_X, MAP_FSIZE_Y };
	}

	//trueでウィンドウモード、falseでフルスクリーンモード
	ChangeWindowMode(windowMode_);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}
	//UD Digi Kyokasho NK - B Bold
	ChangeFont("UD デジタル 教科書体 N-B");

	//マウスの表示設定
	mouseFlag_ = true;
	SetMouseDispFlag(true);

	// キー制御初期化
	SetUseDirectInputFlag(true);

	//エフェクトシーカーの初期化
	InitEffekseer();

	//デバッグクラス
	Debuger::CreateInstance();

	//入力管理初期化
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	//エフェクト管理初期化
	EffectManager::CreateInstance();

	//サウンド管理初期化
	SoundManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

//エフェクトの初期化
void Application::InitEffekseer()
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	// Effekseerを使用するには必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	Size2D size = GetScreenSize();
	Effekseer_Set2DSetting(size.x, size.y);
//	Effekseer_InitDistortion();
}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& debuger = Debuger::GetInstance();

	// ゲームループ
	// && !CheckHitKey(KEY_INPUT_ESCAPE)
	while (ProcessMessage() == 0 && !isGameEnd_)
	{

		inputManager.Update();
		if (GetJoypadInputState(DX_INPUT_PAD1) ||
			GetJoypadInputState(DX_INPUT_PAD2))
		{
			//マウスの表示設定
			SetMouseFlag(false);
			SetMousePoint(screenSize_.x / 2, screenSize_.y / 5);

		}
		else if (CheckHitKeyAll())
		{
			//マウスの表示設定
			SetMouseFlag(true);
		}

		sceneManager.Update();

		debuger.Update();

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{
	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	Debuger::GetInstance().Destroy();
	
	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;
}

//リソースの開放
void Application::Release(void)
{

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

}

//スクリーンのサイズを取得
Size2D Application::GetScreenSize() const
{
	return screenSize_;
}

//マップのサイズを取得
Size2D Application::GetMapSize()const
{
	return mapSize_;
}

//現在のウィンドウモードを取得
bool Application::IsWidowMode()const
{
	return windowMode_;
}

void Application::SetMouseFlag(bool flg)
{
	mouseFlag_ = flg;
	SetMouseDispFlag(mouseFlag_);
}

bool Application::IsMouseFlag(void) const
{
	return mouseFlag_;
}

void Application::EndGame(void)
{
	isGameEnd_ = true;
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	windowMode_ = true;
	isGameEnd_ = false;
	mapSize_ = Size2D();
	mouseFlag_ = false;
	screenSize_ = Size2D();
}