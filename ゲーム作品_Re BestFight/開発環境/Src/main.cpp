#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <DxLib.h>
#include "Application.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		// 初期化失敗
		OutputDebugString("インスタンスの初期化失敗");
		return -1;
	}

	// 実行
	instance.Run();

	//リソースの開放
	instance.Release();

	if (instance.IsReleaseFail())
	{
		//リソース開放失敗
		OutputDebugString("リソースの開放に失敗しました");
		return -1;
	}

	// 解放
	instance.Destroy();

	return 0;
}

//メモ---------------------
// 
// ポストエフェクトを入れる
// 
// ステージ豪華
//		例えば、簡単なテクスチャで観客を作る
//		ストーリー性
// 
// 音だけでなく声でファイトという
//		フリー素材で見つけてくる
//		戦いの始まりも、豪華にする
//		
// 
// スクリーンの大きさ
//		GetWindowRect(GetDesktopWindow(), &rc);
// 
// 
//-------------------------