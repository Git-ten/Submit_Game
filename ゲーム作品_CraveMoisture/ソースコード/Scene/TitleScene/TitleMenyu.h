#pragma once
#include "../MenyuBase.h"
class TitleMenyu : public MenyuBase
{
public:

	TitleMenyu(void);
	~TitleMenyu(void)override;

	// データのロード
	void LoadSceneData(void)override;

	bool IsCollMenyu(void);

	Position2D GetPos(void)override;

private:

	// セレクト種類
	enum class SELECT_KIND
	{
		NONE,
		NO,
		YES,
		MAX,
	};

	// メニュー枠
	std::unique_ptr<Image>imgMenu_;

	// メニュー内の「チュートリアルに進みますか？」文字
	std::unique_ptr<Image>imgTutorial_;

	// メニュー内の現在セレクトしている部分を表す円
	std::unique_ptr<Image>imgCircle_;
	// 現在選択中の種類
	SELECT_KIND selectKind_;


	// 特別性の高い更新
	void UpdateSpecial(void)override;

	void InitOpenMenyu(void)override;

	// サウンドのランチャーを初期化
	void InitSoundPlayLaunch(void)override;

	// 背景画像の描画
	void DrawBack(void)override;

};

