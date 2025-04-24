#pragma once
#include <functional>

class Fader
{
private:

	// フェードの状態
	enum class FADER_STATE
	{
		NONE,
		FADE_OUT,	//暗転
		FADE_IN,	//明転
	};

public:

	// コンストラクタ
	Fader(void);
	// デストラクタ
	~Fader(void);

	// 更新
	void Update(void);
	// 描画
	void Draw(void);

	// フェードの開始
	void FadeStart(void);

	// 画面が完全に暗いか
	const bool IsScreenBrack(void)const;

	// フェードアウト中か
	const bool IsFadeOut(void)const;

	// フェードの終了状態
	const bool IsFadeEnd(void)const;

private:

	// フェードの更新
	using FADER_UPDATE = std::function<void(void)>;

private:

	// フェードの状態
	FADER_STATE nowState_;

	// フェードの更新関数ポインタ
	FADER_UPDATE faderUpdate_;

	// 透明度
	float alpha_;

	// 暗転時に後ろに表示するテクスチャ
	int fadeBackTexture_;


	//何もしない
	//フェイドの状態を変更
	void ChangeFadeState(const FADER_STATE& state);

	// 暗転時の背景描画
	void DrawFadeBack(void);

	void UpdateFadeNone(void);
	//フェイドアウト
	void UpdateFadeOut(void);
	//フェイドイン
	void UpdateFadeIn(void);

};