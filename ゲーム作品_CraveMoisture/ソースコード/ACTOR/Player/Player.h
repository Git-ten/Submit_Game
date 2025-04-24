#pragma once
#include "../../Scene/GUI/Image.h"
#include "../../Scene/GUI/ImageDiv.h"
#include "PlayerData.h"
#include <functional>
#include "PlayerControle.h"
#include "PlayerState.h"
#include "PlayerWeapon/PlayerWeaponSword.h"
#include "../ActorBase.h"

class Player : public ActorBase
{
public:

	Player(void);
	~Player(void)override;

	void InitSpecial(void)override;

	// バトルスタート処理(適用するかは各々の継承先で決める)
	void ActorBattleStart(void)override;

	// コールバック関数
	bool BattleStepCondition(void)override;

	bool IsDash(void)override;
	bool IsCharge(void)override;
	bool IsTechnique(void)override;
	bool IsParysOrJustAvoid(void)override;
	bool IsSkill(void)override;

private:

	// エフェクトの使用用途
	// int型に変換して使用する
	enum class EFFECT_USES
	{
		MOVE,
		ACTION_SMOKE,
		DASH_SONIC,

		ATTACK_SLASH,
		ATTACK_SLASH_FLASH,
		ATTACK_SLASH_ACCEL,
		PARYS,
		SPARK,
		ATTACK_THUNDER,
		ATTACK_LIGHTNING,
		FINISH_SKILL_THUNDER,
		CHARGE,
		REPAIR,

		DAMAGE_SPARK,
	};

	struct DataTechnique
	{
		float recastTimeMax = 20.0f;	// リキャストタイムの最大値
		Time techniqueTimer;// リキャストタイム
		bool isCanTechnique = true;
	};

	// プレイヤーの操作
	std::unique_ptr<PlayerControle>playerControle_;

	PlayerState::STATE playerState_;

	PlayerData playerData_;

	// テクニック技のデータ
	DataTechnique playerTechnique_;

	// 武器
	std::unique_ptr<PlayerWeaponSword>playerWeapon_;

	// プレイヤーの攻撃ラグ
	float playerAttackRag_;

	// アイコン描画用::リキャストタイム用の数字画像
	std::unique_ptr<ImageDiv>imgTimesBig_;
	std::unique_ptr<ImageDiv>imgTimesSmall_;
	// 二桁目は座標だけ受け取る
	Position2D timesBig_Digit_10_Pos_;
	Position2D timesSmall_Point_10_Pos_;

	// デフォルトの攻撃力
	float defAttackPower_;

	// スキルを使用可能か
	bool isSkill_;

	// スローモーションのタイマー
	Time slowTime_;
	float slowTimeMax_;

	// 無敵状態か
	bool isInvincible_;

	bool isGravity_;// 重力を適用するか
	bool isInertia_;// 慣性を適用するか

	// 時間と総ダメージを計測する
	Time nowGameTime_;
	float nowSumDamage_;

	// 初期化関数
	void InitActorTransform(void)override;
	void InitActorAnimation(void)override;
	void InitActorUi(void)override;
	void InitActorEffectLaunch(void)override;
	void InitActorEffectInfo(void)override;
	void InitActorCameraControl(void)override;
	void InitActorSoundLaunch(void)override;

	// 当たり判定時の処理
	void CollisionedGravityActor(Position3DF pos)override;
	void CollisionedHorizontalActor(void)override;
	void CollisionedDamageActor(float* damagePower)override;

	void UpdateSpecial(void)override;


	// スローモーションのセット(引数時間だけスローになる)
	void SetSlowTime(const float time);
	// スローモーションの更新
	void UpdateSlowTime(void);


	// 移動、アクション
	void Walk(void);

	void Move(void);
	
	void Attack(void);
	void Attack_1(bool& isLerpCamera);
	void Attack_2(bool& isLerpCamera);
	void Attack_3(bool& isLerpCamera);
	void Attack_4(bool& isLerpCamera);
	void Attack_5(bool& isLerpCamera);
	void Attack_6(bool& isLerpCamera);
	void Attack_NormalCharge(bool& isLerpCamera);
	void NormalChargeRun(bool& isLerpCamera);
	void ForwardChargeHorizontalChargeRun(bool& isLerpCamera);

	void Attack_ForwardCharge(bool& isLerpCamera);
	void Attack_HorizontalCharge(bool& isLerpCamera);
	void Attack_Sky_1(bool& isLerpCamera);
	void Attack_Sky_2(bool& isLerpCamera);
	void Attack_Fall(bool& isLerpCamera);


	void Jump(void);
	void Avoid(void);
	void Technique(void);
	void Skill(void);
	void Flinch(void);
	void Death(void);

	// 特定のエフェクトとサウンドを任意でストップ
	void StopAttackEffect(void)override;
	void StopWaveSound(void)override;

	// エネミーへの強制ロックオン
	// 仕様::攻撃時に自動で最も近いエネミーをロックオンする
	void LockOnEnemy(bool isLerp = false);

	// 状態ごとの終了処理
	void ControleEnd(void);
	void EndTechnique(void);
	void EndSkill(void);
	void EndAvoid(void);
	void EndMove(void);
	void EndAttack(void);
	void EndJump(void);
	void EndFlinch(void);
	void EndWalk(void);

	// スキルゲージをチャージ
	void ChagePlayerSkill(float mul = 1.0f);

	// プレイヤーのHPを回復
	void RepairPlayerHp(void);

	void SetUpInertia(Dir3DF dir, float power);

	// 重力と慣性を無効化
	void EndGravity(void);
	void EndInertia(void);

	void DrawSpecial(void)override;

	// アイコン描画用::残りのリキャストタイムを描画
	void DrawIconRecastTime(void)override;

};