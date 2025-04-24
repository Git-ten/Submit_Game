#pragma once
#include "SceneBase.h"

class Transform;
class AnimationController;
class Sound;

class AboutGameScene :public SceneBase
{
public:

	enum class MODE
	{
		TITLE_ABOUT_GAME,//タイトル画面で開かれた
		GAME_STOPMENYU,//戦闘中で開かれた
	};

	//コンストラクタ
	AboutGameScene(const MODE& mode);

	//デストラクタ
	~AboutGameScene(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//メモリ開放
	void Release(void)override;

	//戦闘中に説明画面を開く処理
	void OpenAboutGameScene(void);
	//戦闘中に戻るボタンを押した
	bool IsEnd(void);

private:

	//アクターが動き出す時間
	static constexpr int ACTOR_STEP_MAX = 100;
	//デモが流れるまでの時間
	static constexpr float DEMO_PLAY_TIME = 30.0f;
	//ページ数
	static constexpr int PAGE_MAX = 13;
	//キーの長押し最大時間
	static constexpr float KEY_MAX_TIME = 1.5f;
	//動画のマップサイズ
	static constexpr Size2D MOVIE_SIZE = { 13, 6 };
	//弱攻撃動画のマップ座標
	static constexpr Position2D MOVIE_WEEK_POS = {8, 3};

	//モード
	MODE mode_;

	//戦闘中モード:説明画面を閉じるかどうか
	bool endFlag_;

	//格闘家
	std::unique_ptr<Transform>plTrans1_;
	std::unique_ptr<AnimationController>plAnim1_;
	//剣士
	std::unique_ptr<Transform>plTrans2_;
	std::unique_ptr<AnimationController>plAnim2_;
	//魔術師
	std::unique_ptr<Transform>plTrans3_;
	std::unique_ptr<AnimationController>plAnim3_;

	//現在のページ
	//要素番号として使用するため、0からスタート
	int nowPageCounter_;

	//タイトルに戻る文字
	int imgReturn_[2];
	//戻るボタン画像
	int imgReturnGage_;
	//戻るか否か
	bool returnFlag_;
	//タイトルに戻るキーの長押し時間
	float retTitleTime_;

	//選択音
	std::weak_ptr<Sound>decisionSod_;
	//説明用画像のハンドル番号を格納
	int imgExplain_[PAGE_MAX];
	//移動キー画像(左)
	int imgLeftKey_;
	//左移動キーを押しているかどうか
	bool leftFlag_;
	//移動キー画像(右)
	int imgRightKey_;
	//右移動キーを押しているかどうか
	bool rightFlag_;

	//格闘家弱攻撃動画ハンドル
	int fighterWMovie_;
	//剣士弱攻撃動画ハンドル
	int sordmanWMovie_;
	//魔導士弱攻撃動画ハンドル
	int magicianWMovie_;
	//格闘家強攻撃動画ハンドル
	int fighterSMovie_;
	//剣士弱攻撃動画ハンドル
	int sordmanSMovie_;
	//魔導士弱攻撃動画ハンドル
	int magicianSMovie_;
	//格闘家必殺動画ハンドル
	int fighterSkiMovie_;
	//剣士必殺動画ハンドル
	int sordmanSkiMovie_;
	//魔術師必殺動画ハンドル
	int magicianSkiMovie_;



	//内部でしか使わない関数
	//キャラモデルの初期化
	void InitActor(void);
	//ゲーム説明画像の初期化
	void InitExplain(void);
	//キャラの動画の初期化
	void InitMovie(void);

	//キャラの描画
	void DrawActor(void);
	//ゲーム説明画像の描画
	void DrawExPlain(void);
	//戻るボタンの描画
	void DrawReturn(void);
	//ページめくり用画像の描画
	void DrawKey(void);
	//現在ページの描画
	void DrawPage(void);
	//キャラの動画を描画
	void DrawMovie(void);

};