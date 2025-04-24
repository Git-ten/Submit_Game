#pragma once
#include"../Utility/VectorHeader.h"
#include<map>
#include "SceneBase.h"

class Sound;

class ResultMenyu : public SceneBase
{
public:

	enum class STATE
	{
		FADE_IN,
		SELECT_MENYU,
		RETURN_TITLE,
		RETURN_SELECT,
	};

	//コンストラクタ
	ResultMenyu(void);

	//デストラクタ
	~ResultMenyu()override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//メモリ開放
	void Release(void)override;

	//決定した状態を取得
	STATE GetDecision(void)const;

private:

	//メニューのサイズ
	//ウィンドウモードの場合
	static constexpr Size2D MENYU_WSIZE = { 374, 288 };
	//フルスクリーンモードの場合
	static constexpr Size2D MENYU_FSIZE = { 748, 310 };
	//フェイドの速度
	static constexpr float SPEED = 16.0f;

	//セレクト:タイトルのマップ座標
	static constexpr Position2D TITLE_MAP_POS = { 6, 4 };

	//メニュー背景
	int imgMenyu_;

	//現在の状態
	STATE state_;
	//決定した状態
	STATE decisionState_;

	//文字をシフトするか
	std::map<STATE, bool>shiftFlag_;

	//現在のサイズ
	Size2D nowSize_;
	//決定音
	std::weak_ptr<Sound>decisionSod_;



	//内部でのみ使用する関数
	//メニューのフェイドイン
	void UpdateFadeIn(void);
	//メニューのセレクト処理
	void UpdateSelect(void);

	//描画
	void DrawMenyu(void);

};

