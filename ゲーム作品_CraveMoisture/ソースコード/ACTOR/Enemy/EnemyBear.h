#pragma once
#include <memory>
#include "EnemyState.h"
#include "EnemyBase.h"

class EnemyBear : public EnemyBase
{
public:

	// コンストラクタ
	EnemyBear(void);
	// デストラクタ
	~EnemyBear(void)override;

	void InitSpecial(void)override;

	// コールバック関数
	bool BattleStepCondition(void)override;

private:
	// エフェクトの使用用途
	// int型に変換して使用する
	enum class EFFECT_USES
	{
		// 攻撃エフェクト
		NORMAL_ATTACK,
	};

	// 初期化
	void InitActorTransform(void)override;
	void InitActorEffectLaunch(void)override;
	void InitActorAnimation(void)override;

	void UpdateSpecial(void)override;

	// 攻撃
	void EnemyAttack(void)override;

	void DrawSpecial(void)override;

};