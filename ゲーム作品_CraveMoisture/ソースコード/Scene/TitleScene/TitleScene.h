#pragma once
#include "../GUI/Image.h"
#include "../GUI/ImageGUI.h"
#include "../../Common/Time.h"
#include "../SceneBase.h"
#include "TitleMenyu.h"

class TitleScene : public SceneBase
{
public:

	// コンストラクタ
	TitleScene(void);
	// デストラクタ
	~TitleScene(void)override;

	// データのロード
	void LoadSceneData(void)override;

private:

	// 全体背景
	std::unique_ptr<Image>imgBack_;

	// タイトル名画像
	std::unique_ptr<Image> imgTitleName_;

	// PushAnyButton画像
	std::unique_ptr<Image> imgPushAnyButton_;

	// シャットダウンキー画像
	std::unique_ptr<Image>imgShutDownKey_;

	// 時間計測
	std::unique_ptr<Time> pixcelTime_;

	std::unique_ptr<TitleMenyu>titleMenyu_;

	// 特別性の高い初期化処理
	void InitSpecial(void)override;


	// 特別性の高い更新
	void UpdateSpecial(void)override;

	// PixcelRendererの初期化
	void InitPixcelRenderer(void)override;
	// PixcelRendererの更新
	void UpdatePixcelRenderer(void)override;
	// サウンドのランチャーを初期化
	void InitSoundPlayLaunch(void)override;


	// BGMの再生処理
	void PlayBgm(void)override;

	// PushAnyButtonのアニメーション処理
	void PushAnyButtonAnimation(void);


	// 特別性の高い描画
	void DrawSpecial(void)override;

	// 背景画像の描画
	void DrawBack(void)override;
	// TitleNameの描画
	void DrawTitleName(void);
	// PushAnyButtonの描画
	void DrawPushAnyButton(void);

};