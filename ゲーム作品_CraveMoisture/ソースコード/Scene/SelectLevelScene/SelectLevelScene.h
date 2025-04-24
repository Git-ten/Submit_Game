#pragma once
#include "SelectLevelMenyu.h"
#include "../SceneBase.h"

class SelectLevelScene : public SceneBase
{
public:

	SelectLevelScene(void);
	~SelectLevelScene(void)override;

	// データのロード
	virtual void LoadSceneData(void);

private:

	std::unique_ptr<SelectLevelMenyu>selectMenyu_;

	// 特別性の高い更新
	void UpdateSpecial(void)override;


	// 特別性の高い描画
	void DrawSpecial(void)override;


};