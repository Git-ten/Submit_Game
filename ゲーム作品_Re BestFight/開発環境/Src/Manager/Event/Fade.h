#pragma once
#include<functional>

class Fade
{
public:

	//フェイドの状態
	enum class FADE_STATE
	{
		NONE,
		FADE_OUT,//暗転
		FADE_IN, //明転
	};

	//コンストラクタ
	Fade(void);
	//デストラクタ
	~Fade(void);

	//更新
	void Update(void);
	//描画
	void Draw(void);

	//フェイドの開始
	void FadeStart(void);
	void FadeStart(const FADE_STATE& state);

	//フェイドの状態を変更
	void ChangeFadeState(const FADE_STATE& state);

	//フェイドの速度をセット
	void SetFadeSpeed(float speed);
	//フェイドの速度をデフォルトに戻す
	void DefaultFadeSpeed();

	//フェイド中かどうかを取得
	bool IsFadeFlag(void);

	//フェイド終了をセット
	void SetIsEndFlag(bool flg);
	//フェイドが終了しているか
	bool IsEndFlag(void);

	//フェイドの現在の状態を取得
	FADE_STATE GetNowState(void)const;

private:

	//フェイドの速度
	static constexpr float DEFAULT_FADE_SPEED = 10.0f;
	//暗転時の最大アルファ値
	static constexpr float ALPHA_MAX = 255.0f;
	//明転時の最小アルファ値
	static constexpr float ALPHA_MIN = 0.0f;

	using FadeFunc = std::function<void(void)>;

	FadeFunc fadeUpdate_;

	//フェイド中
	bool IsFadeFlag_;

	//フェイドを終了するか
	bool IsEndFade_;

	//暗転・明転を手動で行うか
	bool IsManualFlag_;

	//フェイドの状態
	FADE_STATE state_;

	//アルファ値
	float alpha_;

	//フェイドの速度
	float speed_;



	//内部でのみ使用する関数
	//何もしない
	void UpdateFadeNone(void);
	//フェイドアウト
	void UpdateFadeOut(void);
	//フェイドイン
	void UpdateFadeIn(void);

};