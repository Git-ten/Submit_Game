#pragma once
#include <vector>
#include <unordered_set>
#include "../../Renderer/ModelRenderer/ModelShadowRenderer.h"
#include "../../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include "../SceneBase.h"
#include "../../ACTOR/ActorBase.h"
#include "BattleManager.h"
#include "PauseMenyu.h"

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);
	// デストラクタ
	virtual ~GameScene(void)override;

	void Release(void)override;

	// データのロード
	virtual void LoadSceneData(void)override;

protected:

	// 全てのアクターの数
	static constexpr long ALL_ACTOR_NUM = 4;

	enum ACTOR_ID
	{
		NONE = 1 << 0,
		PLAYER = 1 << 1,
		ENEMY = 1 << 2,
		STAGE = 1 << 3,
		SKYDOME = 1 << 4,
	};

	enum BATTLE_STEP
	{
		NONE_BATTLE,
		BATTLE_START,
		BATTLE,
		BATTLE_WIN,
		BATTLE_GAMEOVER,
	};

	// シーンのストップ
	bool isSceneStop_;

	BATTLE_STEP battleStep_;
	std::unique_ptr<BattleManager>battleManager_;

	std::unique_ptr<PauseMenyu>pause_;

	// 登録されているACTORはすべて当たり判定処理される
	ACTOR_ID actorCollisionId_;

	std::vector<std::unique_ptr<ActorBase>>actors_;

	std::vector<std::shared_ptr<Image>>actorsUi_;

	// バトル終了時に次のシーンへ移行するまでのタイマー
	Time nextSceneTime_;

	// ゲームオーバー文字画像
	std::unique_ptr<Image>imgGameOver_;



	// ローディング処理中か
	bool isLoadData_;

	// ロード背景画像
	std::unique_ptr<Image> imgLoadBack_;

	//ロードゲージ
	std::unique_ptr<Image> imgLoadGage_;
	// ロードゲージの枠
	std::unique_ptr<Image> imgLoadGageCase_;
	//残りゲージナンバー画像
	std::unique_ptr<ImageDiv> imgLoadPercent_;
	//単位記号('%'だけを使う)
	std::unique_ptr<ImageDiv> imgLoadSymbol_;

	// フェード
	std::unique_ptr<Fader>fader_;

	// 残りゲージ(パーセント単位)
	float loadGageRate_;
	// 残りゲージの最大数(ロード中に変動)
	float loadGageRateMax_;

	// ゲームシーンをストップ
	void StopGameScene(void);
	// ゲームシーンを再生
	void PlayGameScene(void);


	// ゲージをアップ
	void AddGagePercent(const float rate);
	// ゲージの最大値を引数分追加
	void AddGagePercentMax(const float rateMax);


	virtual void SetUpActors(void);

	// 特別性の高い初期化処理
	void InitSpecial(void)override;

	virtual void InitAddSpecial(void);

	// 影シェーダの初期化
	void InitShadowShader(void)override;

	virtual void InitBattleManager(void);

	// 特別性の高い更新
	void UpdateSpecial(void)override;

	virtual void UpdateAddSpecial(void);

	void SetUpEnemyLevelResource(void);

	void InitActors(void);
	void SetUpActorsCollision(void);
	void SetCollision(long& nowCounter, const long size, bool isCanSetCollision);

	void SetUpActorsUi(void);

	// サウンドのランチャーを初期化
	virtual void InitSoundPlayLaunch(void)override;
	void PlayBgm(void)override;

	void Pause(void);

	void DrawPercent(void);

	// 特別性の高い描画
	void DrawSpecial(void)override;

	virtual void DrawAddSpecial(void);

	// すべてのエフェクトの描画処理
	void DrawAllEffect(void);

};