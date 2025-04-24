#pragma once
#include "SceneBase.h"
#include<memory>
#include<functional>

class Transform;
class ActorBase;
class AnimationController;
class MenyuScene;
class Sound;

class TitleScene : public SceneBase
{
public:

	//モードセレクト
	enum class SELECT
	{
		PLAY_GAME,
		ABOUT_GAME,
		SWITCH,
		MAX,
	};

	//シーン
	enum class IN_SCENE_ID
	{
		NONE,
		TITLE_ID,
		MENYU_ID,
	};

	//カーソルの拡大
	//ウィンドウモードの場合
	static constexpr int CARSOL_WSIZE_X = 50;
	static constexpr int CARSOL_WSIZE_Y = 46;
	//フルスクリーンモードの場合
	static constexpr int CARSOL_FSIZE_X = 100;
	static constexpr int CARSOL_FSIZE_Y = 93;

	//文字の拡大
	//ウィンドウモードの場合
	static constexpr int PLAYABOUT_WSIZE_X = 470;
	static constexpr int PLAYABOUT_WSIZE_Y = 60;
	static constexpr int SWITCH_WSIZE_X = 45;
	static constexpr int SWITCH_WSIZE_Y = 45;
	//フルスクリーンモードの場合
	static constexpr int PLAYABOUT_FSIZE_X = 940;
	static constexpr int PLAYABOUT_FSIZE_Y = 121;
	static constexpr int SWITCH_FSIZE_X = 90;
	static constexpr int SWITCH_FSIZE_Y = 90;

	//モデルの大きさ
	static constexpr Size3DF TITLE_MODEL_SCALE = {1.0f,1.0f,1.0f};

	//モデルの座標
	//ウィンドウモードの場合
	static constexpr Position3DF PLAYER_1_WPOS = { -500.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_2_WPOS = { 1300.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_3_WPOS = { 500.0f, -500.0f, 800.0f };
	//フルスクリーンモードの場合
	static constexpr Position3DF PLAYER_1_FPOS = { -200.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_2_FPOS = { 1800.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_3_FPOS = { 850.0f, -500.0f, 800.0f };
	//モデルのサイズ
	//ウィンドウモードの場合
	static constexpr float PLAYER_WSIZE = 3.0f;
	//フルスクリーンモードの場合
	static constexpr float PLAYER_FSIZE = 3.5f;


	//セレクト：プレイゲームのマップ座標
	static constexpr int PLAYGAME_MAP_POSX = 3;
	static constexpr int PLAYGAME_MAP_POSY = 5;
	//セレクト：ゲーム説明のマップY座標
	static constexpr int ABOUT_MAP_POSX = 3;
	static constexpr int ABOUT_MAP_POSY = 7;
	//セレクト：電源ボタンのマップ座標
	static constexpr int SWITCH_MAP_POSX = 15;
	static constexpr int SWITCH_MAP_POSY = 9;

	//コンストラクタ
	TitleScene(void);

	//デストラクタ
	~TitleScene(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//メモリ開放
	void Release(void)override;

private:

	//アクターが動き出す時間
	static constexpr int ACTOR_STEP_MAX = 100;
	//デモが流れるまでの時間
	static constexpr float DEMO_PLAY_TIME = 30.0f;
	
	//電源ボタン
	int imgSwitch_;
	//タイトル背景
	int imgBack_;
	//タイトル画像のハンドルを格納
	int imgTitleHandle_;
	//セレクト画像のハンドルを格納385,520
	int imgSelectHandle_;
	//PlayGame画像
	int imgPlayGameHandle_[2];
	//AboutGame画像
	int imgAboutGameHandle_[2];
	//入力しているか
	std::map<SELECT, bool>inputSelect_;
	//現在合わせているカーソル
	SELECT nowCarsolSelect_;
	//カーソルの位置ごとの座標(2D)
	Position2D carsolMapPos_[INT_CAST(SELECT::MAX)];
	//カーソルのサイズ
	Size2D carsolSize_;
	//文字のサイズ
	Size2D playAboutSize_;
	//電源ボタンの大きさ
	Size2D switchSize_;

	//格闘家
	std::unique_ptr<Transform>plTrans1_;
	std::unique_ptr<AnimationController>plAnim1_;
	//剣士
	std::unique_ptr<Transform>plTrans2_;
	std::unique_ptr<AnimationController>plAnim2_;
	//魔術師
	std::unique_ptr<Transform>plTrans3_;
	std::unique_ptr<AnimationController>plAnim3_;

	//サウンドBGM
	std::weak_ptr<Sound>titleSound_;
	//メニューシーン
	std::unique_ptr<MenyuScene>menyu_;

	//更新関数ポインタ
	using UpdateFunc = std::function<void(void)>;
	using DrawFunc = std::function<void(void)>;
	UpdateFunc update_;
	DrawFunc draw_;

	//現在のシーン
	IN_SCENE_ID nowScene_;
	//デモプレイ再生までのカウンター
	float demoCounter_;
	//決定音
	std::weak_ptr<Sound> decisionSod_;



	//内部でしか使わない関数
	//初期化
	void InitCarsol(void);
	void InitPlayAboutSwitch(void);

	//シーン切り替え
	void ChangeScene(const IN_SCENE_ID& id);

	//タイトル更新
	void UpdateTitle(void);
	//メニュー更新
	void UpdateMenyu(void);
	//タイトル描画
	void DrawSceneTitle(void);
	//メニュー描画
	void DrawSceneMenyu(void);

	//タイトル画像を描画
	void DrawTitle(void);
	//カーソル画像を描画
	void DrawSelect(void);
	//文字画像
	void DrawPlayAboutSwitch(void);

};