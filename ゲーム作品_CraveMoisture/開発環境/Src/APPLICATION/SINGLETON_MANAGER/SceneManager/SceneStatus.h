#pragma once
#include "../../Scene/SceneBase.h"
#include "SceneID.h"
#include <memory>

class SceneStatus
{
public:

	// コンストラクタ
	SceneStatus(void);
	//デストラクタ
	~SceneStatus(void) = default;

	// 現在のシーンの初期化
	void InitScene();
	// 現在のシーンの更新
	void UpdateScene(void);
	// 現在のシーンの描画
	void DrawScene(void);

	// 現在のシーンIDを取得
	const SceneID::SCENE_ID& GetNowSceneState(void)const;

	// シーンの切り替え
	void ChangeSceneState(const SceneID::SCENE_ID& id);

private:

	// 現在のシーン
	SceneID::SCENE_ID nowSceneId_;

	// シーン
	std::unique_ptr<SceneBase>scene_;

};