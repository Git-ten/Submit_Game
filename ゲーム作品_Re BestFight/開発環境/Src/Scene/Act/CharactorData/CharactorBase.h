#pragma once
#include<memory>
#include<map>
#include<functional>
#include"../../../Manager/ResourceManager.h"
#include"../../../Manager/EffectManager.h"
#include"../../../Manager/Effect.h"
#include"../../../Manager/SoundManager.h"
#include"../../../Manager/Sound.h"
#include"../../../Manager/SceneManager.h"
#include"../ActorBase.h"

class Camera;
class UserInterface;
class AnimationController;
class Transform;


class CharactorBase
{

public:

	//デバッグ用のトランスフォーム
	std::shared_ptr<Transform>transform_;

	//デバッグ用のカメラアドレス
	std::weak_ptr<Camera>camera_;

	//デバッグ用のUI
	std::shared_ptr<UserInterface>userInterface_;

	//デバッグ用のアニメーションコントロール
	std::shared_ptr<AnimationController>animController_;

	//デバッグ用の敵インスタンス
	std::weak_ptr<ActorBase>enemy_;

	using EffectData = std::map<EffectManager::EFF, std::vector<std::weak_ptr<Effect>>>;
	using EffectStateData = std::map < EffectManager::EFF, ActorBase::EFFECT_STATE >;
	using DamageAnimFunc = std::function<void(void)>;
	using SoundData = std::map<SoundManager::SOD, std::weak_ptr<Sound>>;

	//エフェクトのベクトル配列
	EffectData effectsMap_;
	EffectStateData effectsState_;

	//サウンドデータ
	SoundData soundsMap_;

	CharactorBase(void);
	virtual ~CharactorBase();

	//初期化
	virtual void Init(std::weak_ptr<ActorBase>enemy) = 0;
	virtual void InitTransform(void) = 0;


	//使用しているモデルのキャラIDを取得
	virtual SceneManager::CHARA_ID GetModelResource(void);
	//攻撃範囲
	virtual float GetAttackArea(void)const;
	//回避力
	virtual float GetAvoidPower(void)const;
	//ジャンプ力
	virtual float GetJumpPower(void)const;
	//防御力
	virtual float GetDefencePower(void)const;
	//自分の色
	virtual Color3D GetMyColor(void)const;
	//オートモード時の攻撃切り替え許可タイミング
	virtual float GetAttackHappen(ActorBase::STATE type)const;
	//速度
	virtual float GetSpeed()const;
	//カメラのセット
	void SetActorCamera(std::weak_ptr<Camera>actCam);

	//必殺技の発動条件
	virtual bool IsUnleashSkill(void)const;
	//必殺技時のカメラ処理
	virtual void SkillCamera(void);
	//弱攻撃フィニッシュ攻撃のカメラ処理
	virtual void WeekFinishCamera(void);
	//強攻撃フィニッシュ攻撃のカメラ処理
	virtual void StrongFinishCamera(void);

protected:

	//内部でのみ使用する関数
	//仮想初期化
	virtual void InitAnim(void) = 0;
	virtual void InitEffect(void);
	virtual void InitEffectState(void);
	virtual void InitSound(void);

};

