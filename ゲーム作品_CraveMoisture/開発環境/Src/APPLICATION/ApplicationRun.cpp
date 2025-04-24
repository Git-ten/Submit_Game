#include <cassert>
#include <DxLib.h>
#include "Singleton_Manager/SceneManager/SceneManager.h"
#include "Singleton_Manager/InputManager/InputManager.h"
#include "Singleton_Manager/EffectResourceManager/EffectResourceManager.h"
#include "ApplicationRun.h"

//#define DRAW_FRAME_RATE
#pragma region 定数宣言

// メインスクリーンの左上座標
const Position2D MAIN_SCREEN_LEFT_POS = { 0, 0 };

#pragma endregion

void ApplicationRun::Running(const Application::ScreenStatus& status)
{
	auto& sceneManager = SceneManager::GetInstance();
	auto& inputManager = InputManager::GetInstance();
	bool isGameEnd = false;

	// フレームレート計測変数を初期化
	float prevFrameTime = static_cast<float>(GetNowCount());
	float nowFrameTime = prevFrameTime;

	// ミリ単位にする
	float frameRate = (1.0f / status.screenFps) * 1000.0f;

	while (ProcessMessage() == 0 && !isGameEnd)
	{
		inputManager.Update();
		sceneManager.Update();

		// 描画先グラフィック領域の指定
		//(３Ｄ描画で使用するカメラの設定などがリセットされる)
		SetDrawScreen(status.mainScreenId);
		// 画面をクリア
		ClearDrawScreen();
		sceneManager.Draw();

#ifdef DRAW_FRAME_RATE
		//フレームレートを描画
		const int screenSizeX = Application::GetInstance().GetScreenSize().x;
		DrawFormatString(screenSizeX - 90, 0, 0xff0000, "FPS[%.1f]", GetFPS());
#endif

		//裏画面に描画
		SetDrawScreen(DX_SCREEN_BACK);
		// 画面をクリア
		ClearDrawScreen();
		// 描画モードをバイリニアフィルタリングに設定
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		if (status.isWidowMode)
		{
			DrawGraph(MAIN_SCREEN_LEFT_POS.x, MAIN_SCREEN_LEFT_POS.y, status.mainScreenId, true);
		}
		else
		{
			DrawExtendGraph(MAIN_SCREEN_LEFT_POS.x, MAIN_SCREEN_LEFT_POS.y, status.desctopSize.x, status.desctopSize.y, status.mainScreenId, true);
		}
		// 描画モードを元に戻す
		SetDrawMode(DX_DRAWMODE_NEAREST);


		// ループにかかった時間を計測
		nowFrameTime = static_cast<float>(GetNowCount());

		// フレームレートをframeRate_に強制的に合わせる
		// 設定されているフレームレート(frameRate_)より進んでいない場合
		const float rate = prevFrameTime + frameRate;
		if (nowFrameTime < rate)
		{
			//時間差分スリープする
			Sleep(static_cast<DWORD>(rate - nowFrameTime));
		}
		prevFrameTime = nowFrameTime;


		ScreenFlip();

		EndGame(isGameEnd);
	}
}

void ApplicationRun::EndGame(bool& flg)
{
	if (SceneManager::GetInstance().IsEndGame())flg = true;
}