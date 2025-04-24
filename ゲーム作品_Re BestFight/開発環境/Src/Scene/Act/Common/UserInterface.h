#pragma once
#include<memory>
#include"../../../Utility/VectorHeader.h"

class Sound;
class Effect;

class UserInterface
{
public:

	//ステータスの最大値
	//通常、下記のSTATEと一緒に使う
	struct MAX_STATE
	{
		//それぞれの最大値
		float hpMax;//ヒットポイントMAX
		float attackMax;//攻撃力MAX
		float staminaMax;//スタミナMAX
		float defenceMax;//防御力MAX
		float avoidMax;//回避力MAX
		float skillMax;//必殺技MAX

		int attackLevelMax;//攻撃レベルMAX
		int defenceLevelMax;//防御レベルMAX
		int avoidLevelMax;//回避レベルMax
	};

	//経験値の最大値
	struct MAX_EXPERIENCE
	{
		float attackExMax;//攻撃経験値の最大値
		float defenceExMax;//防御経験値の最大値
		float avoidExMax;//回避経験値の最大値
	};

	//強化レベルの経験値
	//通常、下記のSTATEと一緒に使う
	struct EXPERIENCE
	{
		float attackEx;//攻撃レベル経験値
		float defenceEx;//防御レベル経験値
		float avoidEx;//回避レベル経験値
		MAX_EXPERIENCE max;//最大経験値
	};

	//ステータスパラメータ
	struct STATE
	{
		float hp;//ヒットポイント
		float attack;//攻撃力
		float stamina;//スタミナ
		float defence;//防御力
		float avoid;//回避力
		float skill;//必殺技
		MAX_STATE max;//最大値

		//強化レベル
		int attackLevel;//攻撃レベル
		int defenceLevel;//防御レベル
		int avoidLevel;//回避レベル
		EXPERIENCE ex;//経験値
	};

	//強化レベルの最大値
	static constexpr int LEVEL_MAX = 5;
	//技を出せる最低スタミナ
	static constexpr float MIN_STAMINA = 20.0f;
	//必殺技ゲージの最大値
	static constexpr float SKILL_MAX = 100.0f;

	//特殊UI(魔術師)
	//魔力ゲージの最大値
	static constexpr float MAX_MAGIC = 100.0f;

	//コンストラクタ
	UserInterface(float hp, float attack, float stamina, float defence, float avoid, float skill);

	//デストラクタ
	~UserInterface(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);

	//ステータスの値取得
	float GetStateStamina(void)const;
	float GetStateHp(void)const;
	float GetStateAttackPower(void)const;
	float GetStateAvoidPower(void)const;
	float GetStateDefenceMax(void)const;
	float GetStateSkill(void)const;
	int GetStateAttackLevel(void)const;
	int GetStateDefenceLevel(void)const;
	int GetStateAvoidLevel(void)const;
	float GetStateMaxStamina(void)const;
	float GetStateMaxHp(void)const;

	//チャージ
	void ChargeStateStamina(float stamina);
	void ChargeStateSkill(float skill);
	void ChargeStateAttackEx(float ex);
	void ChargeStateDefenceEx(float ex);
	void ChargeStateAvoidEx(float ex);

	//ステータスの値の増減
	void SubHp(float sub);
	void SubStamina(float sub);
	void AddStamina(float add);
	void AddSkill(float add);
	void SubDefence(float sub);
	void AddDefence(float add);
	void SetDefence(float value);
	//ダメージ量を加算
	void AddDamagePower(float add);
	//ダメージ量をセットにする
	void SetDamagePower(float power);
	//必殺技ゲージ・強化レベルの初期化
	void InitSkillandLevel(void);

	//ミラーフラグをセット
	void SetMirrorFlag(bool flg);
	//ミラーフラグを取得
	bool GetMirrorFlag(void)const;

	//必殺技ゲージマックス時のエフェクト発生
	void SkillMaxStartEff(void);
	//必殺技ゲージ使用時のエフェクト停止処理
	void SkillMaxEndEff(void);

	//魔力を使用するかをセット
	void SetMagicPower(bool flg);
	//魔力量を加算
	void AddMagicPower(float add);
	//魔力量を減算
	void SubMagicPower(float sub);
	//残り魔力を取得
	float GetMagicPower(void)const;

	//変換
	STATE ToState(float hp, float attack, float stamina,
		float defence, float avoid, float skill,
		const MAX_STATE& max, int attackLev,
		int defenceLev, int avoidLev,
		const EXPERIENCE& ex)const;

	MAX_STATE ToMaxState(float hp, float attack, float stamina,
		float defence, float avoid, float skill,
		int attackLev, int defenceLev, int avoidLev)const;

	EXPERIENCE ToExperienceState(float ex_attack, float ex_defence, float ex_avoid, const MAX_EXPERIENCE& max);
	MAX_EXPERIENCE ToMaxExperienceState(float ex_attack, float ex_defence, float ex_avoid);

private:

	//HPゲージの座標
	static constexpr int HP_GAGE_XPOS = 0;
	static constexpr int HP_GAGE_YPOS = 0;

	//スタミナゲージの座標
	static constexpr int STAMINA_GAGE_XPOS = 0;
	static constexpr int STAMINA_GAGE_YPOS = 45;

	//レベルゲージの座標
	static constexpr int LEVEL_GAGE_XPOS = 70;
	static constexpr int LEVEL_GAGE_YPOS = 400;
	static constexpr float LEVEL_EX_MAX_ADD = 100.0f;
	static constexpr Size2D LEVEL_UP_IMG_SIZE = { 80, 80 };
	//デフォルトの最大レベル経験値
	static constexpr float DEFAULT_LEVEL_MAX = 100.0f;

	//必殺技ゲージの座標
	static constexpr int SKILL_GAGE_XPOS = 120;
	static constexpr int SKILL_GAGE_YPOS = 500;

	//魔力ゲージの座標
	static constexpr int MAGIC_GAGE_XPOS = 0;
	static constexpr int MAGIC_GAGE_YPOS = 85;

	//デバッグ用のステータス
	STATE state_;
	//ダメージ量
	float damagePower_;
	//ダメージフラグ
	float damageColorTime_;

	//表示反転フラグ
	bool mirrorFlag_;

	//ステータス表現画像
	int imgHp_;//HPゲージ
	int imgStamina_;//スタミナゲージ
	int imgCase_;//ケース
	int imgSkill_;//必殺技ゲージ
	int imgSkillCase_;//必殺技ケース
	int imgLevel_;//レベルゲージ
	int imgLevelAttack_;//攻撃力レベルゲージ
	int imgLevelDefence_;//防御力レベルゲージ
	int imgLevelAvoid_;//回避レベルゲージ
	int imgMagic_;//魔力ゲージ
	
	//残り魔力
	float magicPower_;
	//魔力ゲージを扱うかどうか
	bool magicFlag_;

	//スキルMAX時のエフェクト
	std::weak_ptr<Effect>skillEff_;
	//スキルMAX時の効果音
	std::weak_ptr<Sound>skillSod_;



	//内部でしか使わない関数
	//ステータスの値の上限・下限制御
	void LimitState(void);
	//レベルアップ処理
	void LevelUp();

	//描画
	void DrawHp(void);
	void DrawStamina(void);
	void DrawSkill(void);
	void DrawLevel(void);
	void DrawLevelAttack(void);
	void DrawLevelDefence(void);
	void DrawLevelAvoid(void);
	void DrawMagicPower(void);

	//内部計算
	void PercentGageX(int& x, int& y, int& rate, const float& denominator, const float& numerator, const int& img);
	void PercentGageY(int& x, int& y, int& rate, const float& denominator, const float& numerator, const int& img);

};