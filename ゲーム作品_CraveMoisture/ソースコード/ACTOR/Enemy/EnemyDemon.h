#pragma once
#include <memory>
#include "EnemyBase.h"

class EnemyDemon : public EnemyBase
{
public:

	// コンストラクタ
	EnemyDemon(void);
	// デストラクタ
	~EnemyDemon(void)override;

	void InitSpecial(void)override;

	// バトルスタート処理(適用するかは各々の継承先で決める)
	void ActorBattleStart(void)override;

	// コールバック関数
	bool BattleStepCondition(void)override;

private:
	
	// 覚醒状態になっているか
	bool isDemonAwake_;

	// 初期化関数
	void InitActorTransform(void)override;
	void InitActorAnimation(void)override;
	void InitActorEffectLaunch(void)override;
	void InitActorSoundLaunch(void)override;

	void UpdateSpecial(void)override;

	
	// 攻撃
	void EnemyAttack(void)override;
	void EnemyAttack_1(void);
	void EnemyAttack_2(void);
	void EnemyAttack_3(void);
	void EnemyAttack_4(void);
	void EnemyAttack_5(void);
	void EnemyAttack_6(void);
	void EnemyAwakeAttack(void);

	// 覚醒
	void EnemyAwake(void);

	void SpecialStateAnimeAdd(void)override;

	// 円形に広がりながら、当たり判定球を動かしていく
	void SetUpCollEffectSphereDotDir(EffectResourceManager::EFFECT_RESOURCE_ID effectId);

	void HappenNormalClowEffect(EffectResourceManager::EFFECT_RESOURCE_ID effectId);
	void StopAttackEffect(void)override;

	void DrawSpecial(void)override;

};