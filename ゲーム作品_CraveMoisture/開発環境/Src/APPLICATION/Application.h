#pragma once
#include <memory>
#include <string>
#include "../Common/DataLoad/DataPaths.h"
#include "../Common/DataLoad/DataWindows.h"
#include "../Utility/VectorHeader.h"

//シングルトンパターン
class ApplicationRun;

class Application
{
public:

	// スクリーンのステータス
	// 外部からロードする
	struct ScreenStatus
	{
		Size2D screenSize;
		Color3D screenColor;
		Size2D screenCenter;
		Size2D desctopSize;
		int mainScreenId;
		float screenFps;
		bool isWidowMode;
		bool isMouseShow;
		Size2DF diffrence;
		Size2DF reDiffrence;
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);
	// リソースの解放
	void Release(void);
	// リソースの破棄
	void Destroy(void);

	// 実行
	void Run(void);

	// データパスの取得
	const DataPaths& GetDataPaths(void)const;

	// メインスクリーンIDの取得
	const int GetMainScreen(void)const;

	// スクリーンのサイズを取得
	const Size2D& GetScreenSize(void)const;

	// スクリーンの中心座標を取得
	const Position2D& GetScreenCenter(void)const;

	// デスクトップのサイズを取得
	const Size2D& GetScreenDesktopSize(void);

	// スクリーンはデスクトップの何倍か(全画面表示の時のみ有効)
	const Size2DF& GetReDeffrenceSize(void)const;

	// デスクトップはスクリーンの何倍か(全画面表示の時のみ有効)
	const Size2DF& GetDeffrenceSize(void)const;

	// ウィンドウモードを取得
	const bool IsWindowMode(void)const;

	// マウスの表示状態を初期設定にする
	void DefaultMouseDisp(void);

private:

	// 静的インスタンス
	static Application* instance_;

	// スクリーンのステータス
	ScreenStatus screenStatus_;

	// データパス
	static DataPaths dataPaths_;



	// privateでコンストラクタを生成する
	Application(void);
	// コピーコンストラクタも同様
	Application(const Application&);
	//デストラクタ
	~Application(void) = default;

	// ウィンドウの初期化
	void InitWindow(void);
	// DxLibの初期化
	void InitDxLib(void);

	//スクリーンステータスのロード
	void LoadScreenStatus(void);
	// スクリーンのセットアップ
	void SetUpScreenStatus(void);
	// スクリーンのステータスをセッティング
	void SetScreenStatus(void);

	//エフェクシアの初期化
	void InitEffekseer(void);
	// デスクトップのサイズを取得
	const Size2D GetDesktopSize(void);

	//シングルトンインスタンスを生成
	void CreateSingleton(void);

};