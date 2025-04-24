#pragma once
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../GUI/Movie.h"
#include "../SceneBase.h"


class ResultScene : public SceneBase
{
public:

	ResultScene(void);
	~ResultScene(void)override;

	// データのロード
	void LoadSceneData(void)override;

private:

	// 「Time」画像
	std::unique_ptr<Image>imgResultTime_;
	// 「Damage」画像
	std::unique_ptr<Image>imgResultDamage_;

	// 裏で流す動画
	std::unique_ptr<Movie> playerMovie_;

	// リザルト難易度文字画像
	std::unique_ptr<Image>imgResultLevel_;

	SceneManager::GameSceneToResultScene gameToResultData_;

	// 特別性の高い初期化処理
	void InitSpecial(void)override;

	// 特別性の高い更新
	void UpdateSpecial(void)override;

	// 背景画像の描画
	void DrawBack(void)override;

	// 特別性の高い描画
	void DrawSpecial(void)override;

};