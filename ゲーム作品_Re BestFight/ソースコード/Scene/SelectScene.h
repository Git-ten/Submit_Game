#pragma once
#include"../Render/ModelMaterial.h"
#include"../Render/ModelRender.h"
#include "SceneBase.h"
#include<memory>

class Transform;
class ActorBase;
class AnimationController;
class Sound;

class SelectScene : public SceneBase
{
public:

	enum class PLAYER_ID
	{
		PLAYER_1,
		PLAYER_2,
		CPU,
	};

	//コンストラクタ
	SelectScene(bool flg);

	//デストラクタ
	~SelectScene(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//メモリ開放
	void Release(void)override;

private:

	//入力状態
	enum class INPUT
	{
		NONE = -1,

		//開始文字画像の要素番号として使う
		//-----------------
		START_TRGUP = 0,
		START_TRGDOWN = 1,
		//----------------

		//キャラ選択の際に使う
		//-----------------
		CHARA_FIGHTER,
		CHARA_SORDMAN,
		CHARA_MAGICIAN,
		//-----------------

		MAX,
	};

	//初期のセレクト画像座標
	static constexpr Position2D SELECT_CARSOL_WPOS = {218, 425};
	static constexpr Position2D SELECT_CARSOL_FPOS = {436, 808};
	//格闘家座標
	//ウィンドウモードの場合
	static constexpr Position3DF FIGHTER__WPOS = { -100.0f, 100.0f, 600.0f };
	//フルスクリーンモードの場合
	static constexpr Position3DF FIGHTER__FPOS = { 100.0f, 100.0f, 600.0f };
	//マップ座標
	static constexpr Position2D FIGHTER_MAP_POS = { 3, 4 };

	//剣士座標
	//ウィンドウモードの場合
	static constexpr Position3DF SORDMAN_WPOS = { 470.0f, 100.0f, 600.0f };
	//フルスクリーンモードの場合
	static constexpr Position3DF SORDMAN_FPOS = { 900.0f, 100.0f, 600.0f };
	//マップ座標
	static constexpr Position2D SORDMAN_MAP_POS = { 7, 4 };

	//魔術師座標
	//ウィンドウモードの場合
	static constexpr Position3DF MAGICIAN_WPOS = { 1100.0f, 100.0f, 600.0f };
	//フルスクリーンモードの場合
	static constexpr Position3DF MAGICIAN_FPOS = { 1800.0f, 100.0f, 600.0f };
	//マップ座標
	static constexpr Position2D MAGICIAN_MAP_POS = { 12, 4 };

	//キャラのサイズ
	//ウィンドウモードの場合
	static constexpr float CHARA_WSIZE = 2.0f;
	//フルスクリーンモードの場合
	static constexpr float CHARA_FSIZE = 3.0f;

	//開始画像のマップ座標
	static constexpr Position2D START_MAP_POS = {6, 7};
	//開始画像のサイズ
	//ウィンドウモードの場合
	static constexpr Position2D START_WSIZE = { 316, 183};
	//フルスクリーンモードの場合
	static constexpr Position2D START_FSIZE = { 516, 333 };

	//キーの長押し最大時間
	static constexpr float KEY_MAX_TIME = 1.5f;

	//チェーンの速度
	static constexpr int CHAIN_SPEED = 2;
	//チェーンのサイズ
	static constexpr int CHAIN_SIZE_X = 32;
	static constexpr int CHAIN_SIZE_Y = 1080;

	//セレクト画面
	int imgSelect_;
	//チェーン画像
	int imgChain_;
	//チェーンの座標
	Position2D chainPos_;

	//タイトルに戻る文字
	int imgReturn_[2];
	int imgReturnGage_;
	bool returnFlag_;

	//開始文字(入力前と入力後)
	int imgStart_[2];
	//現在の入力状態
	INPUT inputState_;
	//キャラにカーソルを合わせているか
	bool charaCarsolFlag_;
	//キャラ座標をずらすか
	std::map<INPUT, bool>charaPosShif_;
	//選択カーソルの座標
	Position2D selectPos_;
	//決定しようとしているか
	bool decisionFlag_;
	//シーンチェンジフラグ
	bool changeFlag_;

	//現在のプレイヤーID
	PLAYER_ID nowPlayerId_;

	//CPUと戦うか
	bool cpuFlag_;

	//選択中のキャラ
	SceneManager::CHARA_ID charaType_;
	SceneManager::CHARA_ID charaType_2_;

	//格闘家
	std::unique_ptr<Transform>plTrans1_;
	std::unique_ptr<AnimationController>plAnim1_;
	Position3DF plPos1_;
	//剣士
	std::unique_ptr<Transform>plTrans2_;
	std::unique_ptr<AnimationController>plAnim2_;
	Position3DF plPos2_;
	//魔術師
	std::unique_ptr<Transform>plTrans3_;
	std::unique_ptr<AnimationController>plAnim3_;
	Position3DF plPos3_;

	//キャラをすべて選び終わったか
	bool endFlag_;
	//タイトルに戻るキーの長押し時間
	float retTitleTime_;

	//開始音
	std::weak_ptr<Sound>startSod_;
	//選択音
	std::weak_ptr<Sound>decisionSod_;
	std::weak_ptr<Sound>decisionSod_2_;
	std::weak_ptr<Sound>decisionSod_3_;
	//セレクトBGM
	std::weak_ptr<Sound>selectSod_;



	//内部でしか使わない関数
	void InitShader(void);
	//キャラのセレクト処理
	void SelectCharactor(const Position2D& mousePos);
	//スタートボタンのセレクト処理
	void SelectStartButton(const Position2D& mousePos);
	//キャラの決定
	void DecisionCharactor(const INPUT& inputType, SceneManager::CHARA_ID& charaType);
	//キャラの座標シフト
	void ShiftCharactor(const SceneManager::CHARA_ID& type);
	//プレイヤーの変更
	void ChangePlayer(void);
	//選びなおし処理
	void ReturnCharaSelect(void);
	//チェーンスクロールの更新
	void UpdateChain(void);

	//戻るボタンの描画
	void DrawReturn(void);
	//チェーンの描画
	void DrawChain(void);
	//セレクト画像の描画
	void DrawSelect(void);
	//スタート画像の描画
	void DrawStart(void);
	//キャラモデルの描画
	void DrawCharactor(void);

	//キャラそれぞれの描画
	void DrawFighter(void);
	void DrawSordMan(void);
	void DrawMagician(void);

};