#pragma once
#include "../AI/EnemyAIBase.h"
#include "EnemyState.h"
#include "EnemyData.h"
#include "../ActorBase.h"

class EnemyBase : public ActorBase
{
public:

	static constexpr float ENEMT_ATTACK_RAG = 0.6f;

	// アニメーションステップの区切りの割合
	static constexpr float ENEMY_ANIME_DIGIT_MOST_MIN = 1.1f;
	static constexpr float ENEMY_ANIME_DIGIT_MIDLE = 1.5f;
	static constexpr float ENEMY_ANIME_DIGIT_MAX = 2.0f;

	// コンストラクタ
	EnemyBase(void);

	// デストラクタ
	~EnemyBase(void)override;

	virtual void InitSpecial(void)override;

	float NowHp(void)override;

protected:

	// エフェクトの使用用途
	// int型に変換して使用する
	enum class EFFECT_USES
	{
		// 攻撃準備エフェクト
		ATTACK_READY,

		// 攻撃エフェクト
		NORMAL_ATTACK,

		// 攻撃エフェクト(爆)
		ATTACK_BOMB,

		// 攻撃エフェクト(土煙)
		ATTACK_SMOKE,

		// 覚醒(エフェクト)
		AWAKE_1,
		AWAKE_2,
		STAGE_EFFECT,
		AWAKE_ATTACK,

		// ダメージエフェクト
		DAMAGE_SPARK,
	};

	// 攻撃当たり判定する部位(必ず、上からエネミーデータのcollBonesの順番にする)
	enum class ATTACK_BONE
	{
		RIGHT_HAND,
		LEFT_HAND,
		HEAD,
	};

	// エネミーがダウン状態になる期間
	const float DOWN_TIME = 7.0f;

	// レベルに応じて強くなる
	float enemyLevel_;

	EnemyState::STATE enemyState_;

	EnemyData enemyData_;

	// 気絶時のタイマー
	Time enemyDownTime_;

	// エネミーのAI
	std::unique_ptr<EnemyAIBase>enemyAi_;

	// 初期化関数
	virtual void InitActorTransform(void)override;
	virtual void InitActorAnimation(void)override;
	virtual void InitActorUi(void)override;
	void InitActorEffectInfo(void)override;
	void InitActorCameraControl(void)override;
	void InitActorSoundLaunch(void)override;

	void InitDamageEffect(void);
	void InitSpecialSound(void);

	// 当たり判定時の処理関数
	void CollisionedHorizontalActor(void)override;
	virtual void CollisionedGravityActor(Position3DF pos)override;
	void CollisionedDamageActor(float* damagePower)override;

	void SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl)override;

	virtual void UpdateSpecial(void)override;

	// エネミーのパラメータをセッティング
	void SetUpEnemyPalam(void);
	void SetUpEnemyLevelParam(void);

	// アクションの終了条件
	const bool IsEndAction(void)const;

	// 攻撃処理
	virtual void EnemyAttack(void);

	// 移動
	void EnemyMove(void);

	void Flinch(void);
	void Death(void);
	void Down(void);

	// 特殊アニメーションの処理
	void SpecialStateAnime(void);
	virtual void SpecialStateAnimeAdd(void);

	// 当たり判定用球をセッティングする
	void SetUpCollSphere(const ATTACK_BONE& bone, bool isMyBody = false);
	void SetUpCollEffectSphere(EffectResourceManager::EFFECT_RESOURCE_ID effectId);

	virtual void DrawSpecial(void)override;

};