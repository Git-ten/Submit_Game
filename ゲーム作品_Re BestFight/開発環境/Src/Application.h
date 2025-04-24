#pragma once
#include <string>
#include"Utility/VectorHeader.h"

class Application
{
public:

	// スクリーンサイズ
	static constexpr int WINDOW_SCREEN_SIZE_X = 1024;
	static constexpr int WINDOW_SCREEN_SIZE_Y = 640;
	static constexpr int FULL_SCREEN_SIZE_X = 1920;
	static constexpr int FULL_SCREEN_SIZE_Y = 1080;

	//マップ座標のサイズ
	static constexpr int MAP_WSIZE_X = 64;
	static constexpr int MAP_WSIZE_Y = 64;
	static constexpr int MAP_FSIZE_X = 120;
	static constexpr int MAP_FSIZE_Y = 102;

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);
	void InitEffekseer(void);
	// リソースの解放
	void Release(void);
	// リソースの破棄
	void Destroy(void);

	// ゲームループの開始
	void Run(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	//スクリーンのサイズを取得
	Size2D GetScreenSize(void)const;
	//マップのサイズを取得
	Size2D GetMapSize(void)const;

	//現在のウィンドウモードを取得
	bool IsWidowMode(void)const;
	//マウスの表示状態をセット
	void SetMouseFlag(bool flg);
	//マウスの表示状態を取得
	bool IsMouseFlag(void)const;

	//ゲームの終了
	void EndGame(void);

private:

	// 静的インスタンス
	static Application* instance_;

	//ゲーム終了フラグ
	bool isGameEnd_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	//スクリーンの大きさを格納
	Size2D screenSize_;
	//マップの大きさを格納
	Size2D mapSize_;

	//設定したウィンドウモードを格納
	//true = ウィンドウモード,false = フルスクリーンモード
	bool windowMode_;
	//マウスを表示するか
	bool mouseFlag_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application&);

	// デストラクタも同様
	~Application(void) = default;

};