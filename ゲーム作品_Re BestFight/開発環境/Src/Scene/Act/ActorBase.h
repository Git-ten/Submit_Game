#pragma once
#include<memory>
#include<map>
#include<functional>
#include<vector>
#include<DxLib.h>
#include"Common/Transform.h"
#include"Common/UserInterface.h"
#include"Common/LevelUpManager.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/EffectManager.h"
#include"../../Manager/Effect.h"
#include"../../Manager/SoundManager.h"
#include"../../Manager/Sound.h"
#include"../../Utility/VectorHeader.h"

class ActorAi;
class NpcAi;
class Camera;
class UserInterface;
class AnimationController;
class Collider;
class ModelMaterial;
class ModelRender;
class CharactorBase;

#define INT_CAST(a)(static_cast<int>(a))

class ActorBase : public std::enable_shared_from_this<ActorBase>
{
public:

	//プレイヤーの状態
	enum class STATE
	{
		NONE = -1,
		//待機状態
		IDLE,
		//移動
		MOVE,

		//弱攻撃---------
		WEEK_1,
		WEEK_2,
		WEEK_3,
		WEEK_4,
		WEEK_5,
		WEEK_6,
		//--------------

		//強攻撃----------
		STRONG_1,
		STRONG_2,
		STRONG_3,
		STRONG_4,
		//---------------

		//回避
		AVOID,
		//バックステップ
		BACKSTEP,

		//ジャンプ
		JUMP,
		//防御
		DEFENCE,

		//特殊
		//高速移動
		QUICK_MOVE,
		//落下攻撃
		FALL_ATTACK,
		//必殺技
		UNLEASH_SKILL,

		//カウンター攻撃
		COUNTER_ATTACK,

		//吹っ飛ばされている
		BACK_WARD,

		//よろける
		STAGGER_1,
		STAGGER_2,
		STAGGER_3,

		//ミス
		MISS,
	};

	//使用するエフェクト
	//列挙型を配列の要素番号として使用する
	//同じエフェクトのリソースを二つ以上使うとき
	//リソースを増やさずにvector配列で同じID二つをロードする
	//resource[STRONG], resource[STRONG_2]のように使う
	enum class USE_EFFECT
	{
		JUMP = 0,

		SWING = 0,

		FLASH = 0,

		STRONG = 0,
		STRONG_2,

		SORD = 0,
		SORD_2,
		SORD_3,

		SORD_F = 0,

		ATTACK_WIND = 0,
		SKILL_FLAME = 0,
		LASER_FIRE = 0,

		LIGHTNING = 0,
		SKILL_SORD = 0,

		BLUR = 0,

		LEVEL_UP = 0,

		FALL_ATTACK = 0,

		CRASH_GROUND = 0,

		BARIA = 0,

		QUICK_MOVE = 0,

		JUST = 0,

		LASER_MAGIC = 0,
		LASER_VIOLENCE = 0,
		AURA = 0,
		BIG_BANG = 0,
		FIRE_MINE = 0,
		TRNADE = 0,
		FHANTASM = 0,
		SOGEKI = 0,
		METEO = 0,
		MAHOUJIN = 0,

		GURD_BREAK = 0,
	};

	//エフェクトの情報
	//エフェクトごとに用意する
	struct EFFECT_STATE
	{
		Dir3DF scl;//大きさ
		float speed;//再生速度
	};

	static constexpr float DEFAULT_WEIGHT = 0.01f;

	static constexpr float QUICK_SPEED = 100.0f;//デバッグ用10.0f

	//ローカルカメラ
	std::shared_ptr<Camera>actorCam_;

	//キャラのデータや特殊処理
	std::unique_ptr<CharactorBase>charactor_;

	//デバッグ用のトランスフォーム
	std::shared_ptr<Transform>transform_;

	//デバッグ用のカメラアドレス
	std::weak_ptr<Camera>camera_;

	//デバッグ用の当たり判定インスタンス
	std::unique_ptr<Collider>collider_;

	//デバッグ用のUI
	std::shared_ptr<UserInterface>userInterface_;

	//レベルアップマネージャ
	std::unique_ptr<LevelUpManager>levelUpMng_;

	//デバッグ用のアニメーションコントロール
	std::shared_ptr<AnimationController>animController_;

	//AI
	std::shared_ptr<ActorAi>actAi_;
	std::unique_ptr<NpcAi>npcAi_;

	//デバッグ用の敵インスタンス
	std::weak_ptr<ActorBase>enemy_;

	//更新可能か
	bool updateFlg_;

	//コンストラクタ
	ActorBase();

	//デストラクタ
	virtual ~ActorBase(void);

	//仮想初期化
	virtual void Init(std::weak_ptr<ActorBase>enemy, SceneManager::CHARA_ID id) = 0;
	void InitCamera(void);
	virtual void InitMark(void) = 0;
	//仮想更新
	virtual void Update(void) = 0;
	//仮想描画
	virtual void Draw(void) = 0;

	//ダメージ時に与えられるモーションをセット
	void SetDamageMotion(const std::function<void(void)>&func);
	//ダメージ時のモーション終了処理
	void EndDamageMotion(void);

	//現在の状態を取得
	STATE GetState(void)const;
	//ダメージを受けたかどうか
	bool IsDamageAnimFlag(void)const;
	//地面に当たっているか
	bool IsCollGroundFlag(void)const;
	//3Dモデルに当たっているか
	bool IsCollObjFlag(void)const;

	Position3DF GetPos(void)const;

	//使用しているモデルのリソースIDを取得
	virtual SceneManager::CHARA_ID GetModelResource(void);

	//上方向のベクトルをセット
	void SetJumpVector(const VECTOR& vec);
	//横方向のベクトルをセット
	void SetSwayVector(const VECTOR& vec);

	//体重をセット
	void SetWeight(float weight);
	//体重をデフォルトにする
	void SetDefaultWeight();
	//更新可能フラグをセット
	void SetUpdateFlag(bool flg);
	//座標をセット
	void SetPos(const Position3DF& pos);
	//ストップフレームをセット
	void SetStopFrame(float time);

	//仮のプレイヤーの状態遷移
	void ChangeState(const STATE& state);
	//ベクトルに垂直なポリゴンを作成
	void PoligonVecNormInfo(VERTEX3D* ver, const VECTOR& vec);
	//真上ベクトルに垂直なポリゴンを作成
	void PoligonVecNormUpInfo(VERTEX3D* ver, const VECTOR& vec);
	//文字用のポリゴン
	//void PolligonStrInfo(VERTEX3D* ver, VECTOR tpos, float tu, float tv, Color3D color, float ca);

	//速度をゲット
	virtual float GetSpeed(void)const;
	//攻撃範囲
	virtual float GetAttackArea(void)const;
	//回避力
	virtual float GetAvoidPower(void)const;
	//ジャンプ力
	virtual float GetJumpPower(void)const;
	//防御力
	virtual float GetStateDefenceMax(void)const;
	//攻撃力
	virtual float GetAttackPower(void)const;
	//自分の色
	virtual Color3D GetMyColor(void)const;
	//当たり判定の発生タイミング
	virtual float GetAttackHappen(void)const;

	//CPUであるか
	bool GetCpuFlag(void)const;
	//プレイヤーIDの取得
	virtual int GetPlayerId(void)const;


	//与える用のダメージモーション----------------------

	//右によろける
	void DamageRightStagger(void);
	//左によろける
	void DamageLeftStagger(void);
	//後ろによろける
	void DamageBackStagger(void);
	//吹っ飛ぶ
	void DamageBlowStagger(void);
	//ガードブレイクされた
	void DamageGurdBreak(void);
	//ダメージモーションが何も与えられていないとき
	void NoneDamage(void);
	//--------------------------------------------------

	//エフェクト再生
	void PlayEffect(const EffectManager::EFF& type, const USE_EFFECT& number, const Position3DF& pos, const Rot3DF& rot);
	//エフェクト停止
	void StopEffect(const EffectManager::EFF& type, const USE_EFFECT& number);
	//エフェクトを再生しているかどうか
	bool IsPlayEffect(const EffectManager::EFF& type, const USE_EFFECT& number);
	//指定のエフェクトの座標をセット
	void SetEffectPos(const EffectManager::EFF& type, const USE_EFFECT& number, const Position3DF& pos);
	//指定のエフェクトの回転をセット
	void SetEffectRot(const EffectManager::EFF& type, const USE_EFFECT& number, const Rot3DF& rot);
	//指定のエフェクトの座標を取得
	Position3DF GetEffectPos(const EffectManager::EFF& type, const USE_EFFECT& number)const;

	//サウンド再生
	void SoundPlay(const SoundManager::SOD& soundType, int playType);
	//サウンド停止
	void SoundStop(const SoundManager::SOD& soundType);

	//プレイヤーの前方の相対座標
	//倍数で距離変化
	virtual Position3DF GetPlayerForwardPos(const float& scl)const;

	//プレイヤーの回転を適切に補正する
	//つまり、180度Y軸回転する
	virtual Rot3DF GetCollectionPlayerRot()const;

	//必殺技の発動条件
	virtual bool IsUnleashSkill(void)const;
	//必殺技時のカメラ処理
	virtual void SkillCamera(void);
	//弱攻撃フィニッシュ攻撃のカメラ処理
	virtual void WeekFinishCamera(void);
	//強攻撃フィニッシュ攻撃のカメラ処理
	virtual void StrongFinishCamera(void);

	//起動していた無限ループエフェクトの停止
	virtual void StopAllEffect(void);

	//カプセル化
	CapsuleF Model2Capsule(void);
	CapsuleF Model2Capsule(float scl, unsigned int r);

	//球化
	SphereF Model2Sphere();
	SphereF Model2Sphere(unsigned int r);

protected:

	static constexpr Size3DF MARK_SCALE = { 0.2f, 0.2f, 0.2f };

	using EffectData = std::map<EffectManager::EFF, std::vector<std::weak_ptr<Effect>>>;
	using EffectStateData = std::map<EffectManager::EFF, EFFECT_STATE>;
	using DamageAnimFunc = std::function<void(void)>;
	using SoundData = std::map<SoundManager::SOD, std::weak_ptr<Sound>>;

	//エフェクトのベクトル配列
	EffectData effectsMap_;
	EffectStateData effectsState_;

	//サウンドデータ
	SoundData soundsMap_;

	//CPUとして戦うか
	bool cpuFlag_;

	//デバッグ用のダメージアニメーションフラグ
	bool damageAnimFlag_;
	//デバッグ用の与えられるダメージモーション
	DamageAnimFunc damageAnim_;

	//頂点マテリアル
	std::unique_ptr<ModelMaterial>material_;
	//頂点レンダー
	std::unique_ptr<ModelRender>render_;

	//一時停止フレームタイム
	float stopFrame_;

	//体重
	float weight_;

	//状態
	STATE state_;
	
	//上方向のベクトル
	VECTOR jumpVec_;

	//横方向のベクトル
	VECTOR swayVec_;

	//当たり判定フラグ
	//種類に限らず、当たったかどうか
	bool collFlag_;
	//3Dモデルに当たったかどうか
	bool collObjFlag_;

	//自身を表す印
	std::unique_ptr<Transform>mark_;



	//内部でのみ使用する関数
	//仮想初期化
	virtual void InitCollider(void) = 0;
	virtual void InitShader(void);
	void InitCharactor(SceneManager::CHARA_ID id);
	void InitCharactorAi(SceneManager::CHARA_ID id);
	//void TextInit(std::vector<int>& texVec, std::string str);//テキストハンドルIDの初期化

	//印の回転処理
	void MarkRotation(void);

	//仮想当たり判定
	virtual void Collision(void) = 0;

	//重力処理
	void Gravity(void);

	//摩擦処理
	void Friction(void);

	//ストップフレームを更新
	void UpdateStopFlame(void);

	//仮想アクターの描画
	virtual void DrawActor(void) = 0;
	virtual void DrawMark(void) = 0;

};