#pragma once
#include"../Manager/SceneManager.h"
#include "SceneBase.h"

class Stage;
class Camera;
class SkyDome;
class ActorBase;
class Sound;
class Effect;
class StopScene;
class AboutGameScene;
class Fade;

class GameScene :public SceneBase
{
public:

	//現在のゲームシーンの状態
	enum class SCENE_STATE
	{
		START,
		PLAY,
		END,
	};

	//コンストラクタ
	GameScene(SceneManager::CHARA_ID player, SceneManager::CHARA_ID enemy);

	//デストラクタ
	~GameScene(void)override;

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
	static constexpr float LIMIT_TIME = 120.0f;

	//最大カウント
	static constexpr float COUNT_DOWN_MAX = 3.9f;
	//拡大率の最大変化速度
	static constexpr float EX_RATE_SPEED = 10.0f;
	//初期の拡大率(%)
	static constexpr float EX_DEFAULT_RATE = 380.0f;
	//戦闘終了文字の最大拡大率
	static constexpr float FINISH_STR_RATE = 180.0f;
	//戦闘終了時間
	static constexpr float END_MAX_TIME = 3.0f;

	//ゲーム説明エクストラ
	int imgExplainEx_;
	//フェイド
	std::unique_ptr<Fade>fader_;

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
	//決定音
	std::weak_ptr<Sound>decisionSod_;
	//勝負あり音
	std::weak_ptr<Sound>finishSod_;
	//カウントダウン音
	std::weak_ptr<Sound>CountDownSod_;
	//スタート音
	std::weak_ptr<Sound>StartSod_;


	//残り時間
	float remainTime_;

	//ポーズするか
	bool poseFlag_;
	std::unique_ptr<StopScene>pose_;

	//ゲーム説明画面
	bool aboutFlag_;
	std::unique_ptr<AboutGameScene>aboutGameScene_;

	//スタート演出の時のカウントダウン
	float countDown_;
	float prevCountDown_;
	//文字の拡大率
	float strExRate_;
	//戦闘スタートフラグ
	bool startFlag_;
	//戦闘終了からリザルト画面までの計測時間
	float endTime_;

	//エフェクト：戦闘開始!
	std::weak_ptr<Effect>goEff_;

	using UpdateFunc = std::function<void(void)>;
	using DrawFunc = std::function<void(void)>;
	//更新関数ポインタ
	UpdateFunc update_;
	//描画関数ポインタ
	DrawFunc draw_;
	//現在のゲームシーンの状態
	SCENE_STATE nowSceneStat_;



	//内部でしか使わない関数
	//状態遷移
	void ChangeState(const SCENE_STATE& stat);
	//カメラ同期
	void SynchroCamera(void);
	//残り時間の描画
	void DrawLimitTime(void);
	//制限時間処理
	void SubRemainTime(void);

	//戦闘開始演出
	void UpdateStart(void);
	void DrawStart(void);
	//戦闘中
	void UpdatePlay(void);
	void DrawPlay(void);
	//戦闘終了演出
	void UpdateEnd(void);
	void DrawEnd(void);

};