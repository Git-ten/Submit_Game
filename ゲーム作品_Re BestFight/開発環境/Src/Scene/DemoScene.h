#pragma once
#include"../Manager/SceneManager.h"
#include "SceneBase.h"

class Stage;
class Camera;
class SkyDome;
class ActorBase;
class Sound;
class StopScene;

class DemoScene :public SceneBase
{
public:

	//コンストラクタ
	DemoScene(SceneManager::CHARA_ID player, SceneManager::CHARA_ID enemy);

	//デストラクタ
	~DemoScene(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//メモリ開放
	void Release(void)override;

private:

	//制限時間(秒単位)
	static constexpr float LIMIT_TIME = 40.0f;

	//プレイヤーID
	SceneManager::CHARA_ID playerId_;
	//敵ID
	SceneManager::CHARA_ID enemyId_;

	//デバッグ用のプレイヤーインスタンス
	std::shared_ptr<ActorBase>player_;
	//デバッグ用のステージインスタンス
	std::unique_ptr<Stage>stage_;
	//スカイドーム
	std::unique_ptr<SkyDome>skyDome_;
	//デバッグ用のエネミーインスタンス
	std::shared_ptr<ActorBase>enemy_;

	//サウンドBGM
	std::weak_ptr<Sound>gameSound_;

	//残り時間
	float remainTime_;



	//内部でしか使わない関数
	//カメラ同期
	void SynchroCamera(void);
	//残り時間の描画
	void DrawLimitTime(void);
	//制限時間処理
	void SubRemainTime(void);

};