#pragma once
#include "SceneBase.h"
class MenyuBase : public SceneBase
{
public:

	MenyuBase(void);
	virtual ~MenyuBase(void);

	// データのロード
	virtual void LoadSceneData(void)override;

	void SetMenyuIsOpen(bool isOpen);

	bool IsOpen(void);

	virtual Position2D GetPos(void);

protected:


	// 特別性の高い初期化処理
	virtual void InitSpecial(void)override;

	// 特別性の高い更新
	virtual void UpdateSpecial(void)override;

	virtual void InitOpenMenyu(void);

	// サウンドのランチャーを初期化
	virtual void InitSoundPlayLaunch(void)override;

	int InputSelectMenyu(bool inputLeft, bool inputRight, int nowSelect, const int start, const int end);

	// 背景画像の描画
	virtual void DrawBack(void)override;

	// 特別性の高い描画
	virtual void DrawSpecial(void)override;

};