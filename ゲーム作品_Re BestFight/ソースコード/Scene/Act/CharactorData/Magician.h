#pragma once
#include "CharactorBase.h"

class Magician : public CharactorBase
{
public:

	Magician(std::weak_ptr<Camera> camera);
	~Magician()override;

	//初期化
	void Init(std::weak_ptr<ActorBase>enemy)override;
	void InitTransform(void)override;


	//使用しているモデルのリソースIDを取得
	SceneManager::CHARA_ID GetModelResource(void)override;
	//攻撃範囲
	float GetAttackArea(void)const override;
	//回避力
	float GetAvoidPower(void)const override;
	//ジャンプ力
	float GetJumpPower(void)const override;
	//防御力
	float GetDefencePower(void)const override;
	//自分の色
	Color3D GetMyColor(void)const override;
	//速度
	float GetSpeed()const override;
	float GetAttackHappen(ActorBase::STATE type)const override;

	//必殺技の発動条件
	bool IsUnleashSkill(void)const override;
	//必殺技時のカメラ処理
	void SkillCamera(void)override;
	//弱攻撃フィニッシュ攻撃のカメラ処理
	void WeekFinishCamera(void)override;
	//強攻撃フィニッシュ攻撃のカメラ処理
	void StrongFinishCamera(void)override;

private:

	static constexpr Size3DF PLAYER_SCALE = { 1.2f, 1.2f, 1.2f };
	static constexpr float JUMP_POWER = 40.0f;//デバッグ用50.0f
	static constexpr Rot3DF DEFAULT_ANGLE = { 0.0, 180.0, 0.0 };
	static constexpr float HP_MAX = 100.0f;//デバッグ用
	static constexpr float STAMINA_MAX = 100.0f;//デバッグ用
	static constexpr float DEFENCE_MAX = 3.0f;//デバッグ用
	static constexpr float SKILL_DEFAULT_MAX = 100.0f;//デバッグ用
	static constexpr float SKILL_MAX = 100.0f;//デバッグ用
	static constexpr float AVOID_POWER = 30.0f;//デバッグ用
	static constexpr float ATTACK_POWER = 1.2f;
	static constexpr float FORWARD_ATTACK_AREA = 900.0f;//デバッグ用
	static constexpr float SPEED = 45.0f;//速度

	static constexpr Position3DF FINISH_SKILL_CAMERA_POS = {200.0f, 50.0f, -700.0f };
	static constexpr Position3DF START_WEEK_CAMERA_POS = { 0.0f, 400.0f, -250.0f };
	static constexpr Position3DF FINISH_WEEK_CAMERA_POS = { 0.0f, 200.0f, -850.0f };
	static constexpr Position3DF FINISH_STRONG_CAMERA_POS = { 0.0f, 200.0f, -450.0f };
	static constexpr Color3D MY_COLOR = { 0.0f, 1.0f, 0.0f };



	//内部でのみ使用する関数
	//仮想初期化
	void InitAnim(void)override;
	void InitEffect(void)override;
	void InitEffectState(void)override;
	void InitSound(void)override;

};

