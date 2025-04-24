#pragma once
#include "../../Scene/GUI/ImageDiv.h"
#include "../MenyuBase.h"

class PauseMenyu : public MenyuBase
{
public:

	PauseMenyu(void);
	~PauseMenyu(void);

	// データのロード
	void LoadSceneData(void)override;

private:

	// セレクト種類
	enum class SELECT_KIND
	{
		NONE,
		RETURN_TITLE,
		RETURN_GAME,
		MAX,
	};

	// ポーズ背景
	std::unique_ptr<Image>imgPauseBack_;

	// 「Time」画像
	std::unique_ptr<Image>imgTime_;
	// 「Damage」画像
	std::unique_ptr<Image>imgDamage_;
	// メニュー内の現在セレクトしている部分を表す円
	std::unique_ptr<Image>imgCircle_;

	// 現在選択中の種類
	SELECT_KIND selectKind_;


	// 特別性の高い初期化処理
	void InitSpecial(void)override;

	// 特別性の高い更新
	void UpdateSpecial(void)override;

	void InitOpenMenyu(void)override;

	// サウンドのランチャーを初期化
	void InitSoundPlayLaunch(void)override;

	// 背景画像の描画
	void DrawBack(void)override;
};