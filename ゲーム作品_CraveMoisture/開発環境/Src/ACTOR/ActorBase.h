#pragma once
#include <memory>
#include <list>
#include "../COMMON/Transform.h"
#include "../Common/Time.h"
#include "../Common/EffectPlayLaunch.h"
#include "../Common/SoundPlayLaunch.h"
#include "../Common/AnimationController.h"
#include "../../Scene/GUI/Image.h"
#include "../../Common/DataLoad/DataCameraControl.h"
#include "ActorData.h"

class ActorBase
{
protected:

	static constexpr float GRAVITY_UP_OFFSET = 300.0f;
	static constexpr float GRAVITY_DOWN_OFFSET = 20.0f;

	static constexpr float GAGE_INIT = 0.0f;

	static constexpr float ATTACK_RAG_MAX = 1.0f;

	static constexpr float HP_MIN = 0.0f;

	static constexpr float EFFECT_DEFAULT_SPEED = 100.0f;

	struct ACTOR_COLLISION
	{
		Position3DF* lockOnPos = nullptr;

		std::weak_ptr<Line3DF>collisionGravity;
		std::weak_ptr<SphereF>collisionSphere;

		std::weak_ptr<SphereF>collisionAttackSphere;
		int* collisiionAttackModelId_;

		int* collisionModelId;
		int* collisionGravityModelId;

		bool isCollAttack_ = false;

		bool isCollHorizontal = true;
		bool isMyselfEnemy = false;

		float* damagePower = nullptr;
	};

public:

	static constexpr float ENEMY_LEVEL_EASY = 0.5f;
	static constexpr float ENEMY_LEVEL_NORMAL = 0.7f;
	static constexpr float ENEMY_LEVEL_HARD = 1.0f;

	// コンストラクタ
	ActorBase(void);
	// デストラクタ
	virtual ~ActorBase(void);

	void Init(void);
	virtual void InitSpecial(void);

	void Update(void);
	virtual void Draw(void);

	void Release(void);

	// スローモーションをセットアップ(自身にしか影響がないもの)
	void SetUpMySlow(bool isSlow);

	// アクター同士の当たり判定情報の参照を追加
	void SetOtherActorCollision(ACTOR_COLLISION& actorColl, const std::list<ACTOR_COLLISION*>& addActorList = std::list<ACTOR_COLLISION*>());
	// 自身の当たり判定情報を取得
	ACTOR_COLLISION& GetMyCollision(void);

	// 自分以外で特別に追加するアクターの当たり判定情報
	const std::list<ACTOR_COLLISION*>& GetAddSpecialActorCollision(void);

	const std::vector<std::shared_ptr<Image>>& GetImagesUi(void);

	void SetOtherActorSlowFunc(const std::function<void(bool)> func);
	const std::function<void(bool)> GetMySlowFunc(void);

	// バトルスタート処理(適用するかは各々の継承先で決める)
	virtual void ActorBattleStart(void);

	// アイコン描画用::残りのリキャストタイムを描画
	virtual void DrawIconRecastTime(void);

	// コールバック関数
	virtual bool BattleStepCondition(void);

	int GetModelId(void);

	Position3DF GetPos(void);

	bool IsTouchGround(void);

	// 少しでも慣性が働いていればtrue
	bool IsInertia(void);

	virtual float NowHp(void);

	// プレイヤーのみに使う
	virtual bool IsDash(void);
	virtual bool IsCharge(void);
	virtual bool IsTechnique(void);
	virtual bool IsParysOrJustAvoid(void);
	virtual bool IsSkill(void);

private:

	// ステートごとのカメラ制御データ
	using CameraControlData = std::map<int, std::vector<DataCameraControl>>;

protected:

	// ステップごとにバトルマネージャーへ返す条件式を変化させる
	int actorBattleStep_;

	// バトルスタートフラグ
	bool isBattleStart_;

	// 自分だけのスローモーションフラグ
	bool isMySlow_;

	// アクターのトランスフォーム
	std::shared_ptr<Transform>actorTransform_;

	// アクターのアニメーション
	std::unique_ptr<AnimationController>actorAnime_;

	ACTOR_COLLISION myActorCollision_;

	// アクター同士の当たり判定情報一覧
	std::list<ACTOR_COLLISION*>otherActorCollisiton_;

	std::list<ACTOR_COLLISION*>addOtherActorCollision_;

	// アクター専用のUI画像
	std::vector<std::shared_ptr<Image>>actorUi_;
	
	// アクターの攻撃ラグ
	std::unique_ptr<Time>actorAttackRagTime_;

	// エフェクトのランチャー
	std::unique_ptr<EffectPlayLaunch>actorEffectLaunch_;
	// エフェクトの情報
	std::map<int, DataEffect>effectInfo_;

	// 効果音のランチャー
	std::unique_ptr<SoundPlayLaunch>waveLaunch_;

	// 移動方向
	Dir3DF actorMoveDir_;

	// 地面に足がついているか
	bool isTouchGround_;

	// 重力
	VECTOR gravity_;
	// 慣性
	VECTOR inertia_;

	// スローモーション中であるか
	bool isSlowActor_;

	// 自分以外のアクターにスローモーションを共有する場合使用
	std::vector<std::function<void(bool)>>otherSlowActor_;

	// カメラワーク
	CameraControlData cameraControle_;

	virtual void SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl);
	
	virtual void InitActorTransform(void);
	virtual void InitActorAnimation(void);
	virtual void InitActorUi(void);
	virtual void InitActorEffectLaunch(void);
	virtual void InitActorEffectInfo(void);
	virtual void InitActorCameraControl(void);
	virtual void InitActorSoundLaunch(void);

	// 指定したステートのカメラ制御データを追加
	void AddStateCameraControlData(const int state, const LoadData::LoadDataInfo& info);

	void ActorCollision(void);
	virtual void CollisionedGravityActor(Position3DF pos);
	virtual void CollisionedHorizontalActor(void);
	virtual void CollisionedDamageActor(float* damagePower = nullptr);

	virtual void StopAttackEffect(void);
	virtual void StopWaveSound(void);

	virtual void UpdateSpecial(void);
	virtual void DrawSpecial(void);

	void Gravity(ActorData& data);
	void Inertia(ActorData& data);

	void SetActorSlow(bool isSlow);

	void ActorMoveVector(ActorData& data, Dir3DF vec, int addPower = 0);
	void ActorMoveDistance(ActorData& data, Dir3DF vec, float dis);
	void ActorMoveSet(ActorData& data, Position3DF pos);

	void ActorOtherLockOnRotation(const ActorData& data, bool isRotCamera = false, bool isRotActor = true, bool isLerpCamera = false);
	float GetDistanceActor(const ActorData& data);

	void ActorRotationDir(const ActorData& data, Dir3DF dir);
	Dir3DF GetActorOtherLockonDir(const ActorData& data, bool isRotCamera = false, 
		const Position3DF& pos = Position3DF(), Position3DF* getPos = nullptr, bool isLerpCamera = false);

	DataEffect GetEffectDataInfo(const int useNumber);

private:

	void ActorMove(ActorData& data, Dir3DF dir, float dis);

};