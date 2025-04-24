#pragma once
#include"ActorBase.h"

class Player : public ActorBase
{
public:

	//コンストラクタ
	Player(std::weak_ptr<Camera> camera, bool flag);

	//デストラクタ
	~Player(void)override;

	//初期化
	void Init(std::weak_ptr<ActorBase>enemy, SceneManager::CHARA_ID id)override;
	void InitMark(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

	SceneManager::CHARA_ID GetModelResource(void)override;

	float GetAttackArea(void)const override;
	float GetAvoidPower(void)const override;
	float GetJumpPower(void)const override;
	float GetStateDefenceMax(void)const override;
	float GetAttackPower(void)const override;
	Color3D GetMyColor(void)const override;
	float GetAttackHappen(void)const override;

	//プレイヤーIDの取得
	int GetPlayerId(void)const override;

	//速度をゲット
	float GetSpeed(void)const override;

	//プレイヤーの前方の相対座標
	//倍数で距離変化
	Position3DF GetPlayerForwardPos(const float& scl)const override;

	//プレイヤーの回転を適切に補正する
	//つまり、180度Y軸回転する
	Rot3DF GetCollectionPlayerRot(void)const override;

	//必殺技の発動条件
	bool IsUnleashSkill(void)const override;

	//必殺技時のカメラ処理
	void SkillCamera(void)override;
	//弱攻撃フィニッシュ攻撃のカメラ処理
	void WeekFinishCamera(void)override;
	//強攻撃フィニッシュ攻撃のカメラ処理
	void StrongFinishCamera(void)override;

	//起動していた無限ループエフェクトの停止
	void StopAllEffect(void)override;

private:

	//定数
	static constexpr Size3DF PLAYER_SCALE = { 1.0f, 1.0f, 1.0f };
	static constexpr float JUMP_POWER = 40.0f;//デバッグ用50.0f
	static constexpr float HP_MAX = 100.0f;//デバッグ用
	static constexpr float STAMINA_MAX = 100.0f;//デバッグ用
	static constexpr float DEFENCE_MAX = 3.0f;//デバッグ用
	static constexpr float SKILL_DEFAULT_MAX = 100.0f;//デバッグ用
	static constexpr float SKILL_MAX = 100.0f;//デバッグ用
	static constexpr float AVOID_POWER = 70.0f;//デバッグ用
	static constexpr float FORWARD_ATTACK_AREA = 300.0f;//デバッグ用

	static constexpr Position3DF START_SKILL_CAMERA_POS = {-100.0f, 100.0f, -450.0f };
	static constexpr Position3DF END_SKILL_CAMERA_POS = {-200.0f, 50.0f, -550.0f };
	static constexpr Position3DF FINISH_SKILL_CAMERA_POS = {-100.0f, 50.0f, -50.0f };
	static constexpr Position3DF FINISH_WEEK_CAMERA_POS = {0.0f, 200.0f, -450.0f };
	static constexpr Position3DF FINISH_STRONG_CAMERA_POS = {0.0f, 200.0f, -450.0f };
	static constexpr Color3D MY_COLOR = {1.0f, 0.0f, 0.0f};

	//ポイントライトの初期座標
	static constexpr Position3DF POINT_LIGHT_POS = { 0.0f, 6000.0f, 8000.0f };
	//ポイントライトの色
	static constexpr Color3D POINT_LIGHT_COLOR = { 0.0f, 6000.0f, 3000.0f };;
	//ポイントライトの強さ
	static constexpr float POINT_LIGHT_POWER = 6000.0f;

	//文字の文字表示時間
	static constexpr float STR_MAX_TIME = 3.0f;
	//文字の点滅
	static constexpr int LV_COUNT_MAX = 50;

	//レベルアップエフェクト
	std::weak_ptr<Effect>carsolAttackEff_;
	std::weak_ptr<Effect>carsolDefenceEff_;
	std::weak_ptr<Effect>carsolAvoidEff_;
	
	//攻撃力アップ時文字表示時間
	//float attackStrCount_;
	//std::vector<int>attackTex_;
	//float defenceStrCount_;
	//std::vector<int>defenceTex_;
	//float avoidStrCount_;
	//std::vector<int>avoidTex_;
	bool attackStrFlag_;
	bool defenceStrFlag_;
	bool avoidStrFlag_;
	//レベルアップ時の数字の点滅
	int LvAtCount_;
	int LvDxCount_;
	int LvAvCount_;
	int prevLvAttack_;
	int prevLvDefence_;
	int prevLvAvoid_;



	//内部でのみ使用する関数
	//初期化
	void InitCollider(void)override;
	void InitShader(void)override;
	//当たり判定
	void Collision(void)override;

	//プレイヤーの描画
	void DrawActor(void)override;
	//マークの描画
	void DrawMark(void)override;
	//レベルアップの描画
	void DrawLevelUp(void);
	//レベルアップの描画(ミラー)
	void DrawLevelUpMirror(void);

};