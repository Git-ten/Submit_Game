#include <cassert>
#include <EffekseerForDxLib.h>
#include "Singleton_Manager/SingletonManagerFactory.h"
#include "../Utility/Utility.h"
#include "../Common/DataLoad/DataWindows.h"
#include "../Common/DataLoad/LoadDataPaths.h"
#include "../Common/DataLoad/LoadDataWindows.h"
#include "ApplicationRun.h"
#include "Application.h"

Application* Application::instance_ = nullptr;
DataPaths Application::dataPaths_ = LoadDataPaths::LoadedDataPaths();

#pragma region 定数宣言

//カラービット数(16bit Or 32bit)
const int COLOR_BIT_NUM = 32;

//画面に表示できるエフェクトの最大パーティクル数
const int EFFECT_PARTICLE_MAX = 9200;

// 例外子
const int EXCEPTION_VALUE = -1;

#pragma endregion

void Application::CreateInstance(void)
{
	assert(instance_ == nullptr || !"Application::CreateInstanceでインスタンスが作成されているのに再び生成しようとしています");

	instance_ = new Application();
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	// スクリーンステータスのロード
	LoadScreenStatus();

	//ウィンドウの初期化
	InitWindow();

	// DxLibの初期化
	InitDxLib();

	// スクリーンのセットアップ
	SetUpScreenStatus();

	//エフェクトシーカーの初期化
	InitEffekseer();

	//シングルトンインスタンスを生成
	CreateSingleton();
}

void Application::InitWindow(void)
{
	// アプリケーションの初期設定
	SetWindowText("Crave Moisture");

	// デスクトップの大きさ
	screenStatus_.desctopSize = GetDesktopSize();

	if (screenStatus_.isWidowMode)
	{
		SetGraphMode(screenStatus_.screenSize.x, screenStatus_.screenSize.y, COLOR_BIT_NUM);
	}
	else
	{
		SetGraphMode(screenStatus_.desctopSize.x, screenStatus_.desctopSize.y, COLOR_BIT_NUM);
	}

	// trueでウィンドウモード、falseでフルスクリーンモード
	ChangeWindowMode(screenStatus_.isWidowMode);

	// スクリーンのカラー
	SetBackgroundColor(screenStatus_.screenColor.x, screenStatus_.screenColor.y, screenStatus_.screenColor.z);

}

void Application::InitDxLib(void)
{
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	assert(DxLib_Init() != EXCEPTION_VALUE || !"DxLibの初期化失敗");

	// キー制御初期化
	SetUseDirectInputFlag(true);
}

void Application::LoadScreenStatus(void)
{
	// ウィンドウの情報をロード
	const DataWindows& dataWindows = LoadDataWindows::LoadedDataWindows(dataPaths_);

	// スクリーンのカラー
	Color3D screenColor = dataWindows.screenColor;

	// フルスクリーン時に違和感なく座標を取得するためのパラメータ
	Size2DF diffrence = Utility::VECTOR2D_ONE;
	Size2DF reDiffrence = Utility::VECTOR2D_ONE;

	// ロードした情報を基にセッティング
	// 画面の中心
	const Size2D& screenCenter = Utility::GetHalfVector2(dataWindows.screenSize);

	//スクリーンのステータスをセット
	screenStatus_ = { dataWindows.screenSize,
					  screenColor,
					  screenCenter,
					  Size2D(),
					  EXCEPTION_VALUE,
					  dataWindows.frameFps,
					  dataWindows.isWindowMode,
					  dataWindows.isMouseShow,
					  diffrence,
					  reDiffrence };
}

void Application::SetUpScreenStatus(void)
{
	// メインスクリーンの作成
	screenStatus_.mainScreenId = MakeScreen(screenStatus_.screenSize.x, screenStatus_.screenSize.y);

	// フルスクリーン時に違和感なく座標を取得するためのパラメータ
	if (!screenStatus_.isWidowMode)
	{
		screenStatus_.diffrence = Vector2F::ToVector2F(screenStatus_.desctopSize) /
			Vector2F::ToVector2F(screenStatus_.screenSize);

		screenStatus_.reDiffrence = Vector2F::ToVector2F(screenStatus_.screenSize) /
			Vector2F::ToVector2F(screenStatus_.desctopSize);
	}


	// マウスの表示設定
	SetMouseDispFlag(screenStatus_.isMouseShow);
}

//エフェクトの初期化
void Application::InitEffekseer(void)
{
	if (Effekseer_Init(EFFECT_PARTICLE_MAX) == EXCEPTION_VALUE)
	{
		DxLib_End();
	}

	// Effekseerを使用するには必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// スクリーンの大きさ
	// Size2D size = screenStatus_.screenSize;
	// Effekseer_Set2DSetting(size.x, size.y);
	// Effekseer_InitDistortion();
}

void Application::SetScreenStatus(void)
{

}

const Size2D Application::GetDesktopSize(void)
{
	RECT Wsize;
	GetWindowRect(GetDesktopWindow(), &Wsize);
	Size2D size = { Wsize.right - Wsize.left, Wsize.bottom - Wsize.top };

	return size;
}

void Application::CreateSingleton(void)
{
	SINGLETON_MANAGER_FACTORY(INPUT_MANAGER);
	SINGLETON_MANAGER_FACTORY(RESOURCE_MANAGER);
	SINGLETON_MANAGER_FACTORY(EFFECT_RESOURCE_MANAGER);
	SINGLETON_MANAGER_FACTORY(SOUND_RESOURCE_MANAGER);
	SINGLETON_MANAGER_FACTORY(SCENE_MANAGER);
}

void Application::Run(void)
{
	ApplicationRun::Running(screenStatus_);
}

const DataPaths& Application::GetDataPaths(void) const
{
	return dataPaths_;
}

const int Application::GetMainScreen(void) const
{
	return screenStatus_.mainScreenId;
}

const Size2D& Application::GetScreenSize(void) const
{
	return screenStatus_.screenSize;
}

const Position2D& Application::GetScreenCenter(void) const
{
	return screenStatus_.screenCenter;
}

const Size2D& Application::GetScreenDesktopSize(void)
{
	return screenStatus_.desctopSize;
}

const Size2DF& Application::GetReDeffrenceSize(void) const
{
	return screenStatus_.reDiffrence;
}

const Size2DF& Application::GetDeffrenceSize(void) const
{
	return screenStatus_.diffrence;
}

const bool Application::IsWindowMode(void) const
{
	return screenStatus_.isWidowMode;
}

void Application::DefaultMouseDisp(void)
{
	SetMouseDispFlag(screenStatus_.isMouseShow);
}

void Application::Destroy(void)
{
	// シングルトンインスタンスをすべて削除
	SingletonManagerFactory::DestroyInstance();

#ifdef DEBUG
	Debuger::GetInstance().Destroy();
#endif

	// DxLib終了
	if (DxLib_End() == EXCEPTION_VALUE)assert(!"DxLibの終了に失敗");

	delete instance_;
}

//リソースの開放
void Application::Release(void)
{

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == EXCEPTION_VALUE)assert(!"DxLibの終了に失敗");

}

Application::Application(void) :
	screenStatus_(ScreenStatus())
{
}