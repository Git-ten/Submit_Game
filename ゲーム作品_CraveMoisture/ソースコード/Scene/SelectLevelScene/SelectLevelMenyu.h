#pragma once
#include "../MenyuBase.h"
class SelectLevelMenyu : public MenyuBase
{
public:

	SelectLevelMenyu(void);
	~SelectLevelMenyu(void)override;

	// データのロード
	void LoadSceneData(void)override;

private:

	// セレクト種類
	enum class SELECT_KIND
	{
		NONE,
		EASY,
		NORMAL,
		HARD,
		MAX,
	};

	// 現在選択中の種類
	SELECT_KIND selectKind_;

	std::unique_ptr<Image>imgSelectLevelBack_;

	std::unique_ptr<Image>imgEnemyIcon_;

	std::unique_ptr<Image>imgCircle_;

	// 特別性の高い初期化処理
	void InitSpecial(void)override;

	// 特別性の高い更新
	void UpdateSpecial(void)override;

	// 背景画像の描画
	void DrawBack(void)override;

};