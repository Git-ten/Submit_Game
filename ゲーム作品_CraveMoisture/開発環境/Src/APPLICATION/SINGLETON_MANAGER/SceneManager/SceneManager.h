#pragma once
#include "../../../Common/DeltaTime.h"
#include "../../../Common/Camera.h"
#include "../../../Common/Fader.h"
#include "SceneStatus.h"

class SceneManager
{
public:

	struct SelectSceneToGameScene
	{
		float enemyLevel = 0.5f;// 0.5～0.7～1.0の間のどれか
	};

	struct GameSceneToResultScene
	{
		float resultTime = 0.0f;
		float resultDamage = 0.0f;
		ResourceManager::RESOURCE_ID levelResource = ResourceManager::RESOURCE_ID::LEVEL_EASY;
	};

public:

	//インスタンスの生成
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SceneManager& GetInstance(void);

	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);

	// リソースの完全破棄
	void Destroy(void);

	// シーンの更新をロック
	void LockSceneUpdate(void);
	// シーンの更新をアンロック
	void UnLockSceneUpdate(void);
	// シーンの更新ロック状態を取得
	const bool IsLockSceneUpdate(void)const;

	// シーンの切り替え
	void ChangeSceneState(const SceneID::SCENE_ID& id);

	// ゲーム終了の信号(trueで終了)
	const bool IsEndGame(void)const;
	// ゲームの終了処理
	void EndGame(void);

	// デルタタイムの取得
	const float GetDeltaTime(void)const;

	// NULLであるか
	static const bool IsNullptr(void);

	// カメラの参照を取得
	const std::weak_ptr<Camera>GetCamera(void)const;

	//ライトの向きを取得
	const Dir3DF& GetLightDir(void)const;

	void SetUpSelectToGameScene(const SelectSceneToGameScene& data);
	SelectSceneToGameScene GetSelectToGameSceneData(void);

	void SetUpGameToResultScene(const GameSceneToResultScene& data);
	GameSceneToResultScene GetGameToResultSceneData(void);

private:

	SelectSceneToGameScene toGameSceneData_;
	GameSceneToResultScene toResultSceneData_;

	// 静的インスタンス
	static SceneManager* instance_;

	// シーンのステータス
	std::unique_ptr<SceneStatus>sceneStatus_;

	// カメラ
	std::shared_ptr<Camera>camera_;

	// フェード
	std::unique_ptr<Fader>fader_;

	//ゲームの終了状態
	bool isEndGame_;

	// デルタタイム
	DeltaTime deltaTime_;

	//ライトの向き
	Dir3DF lightDir_;

	// フェードの起動
	void FadeStart(void);

	// privateでコンストラクタを生成する
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	//デストラクタ
	~SceneManager(void) = default;

};