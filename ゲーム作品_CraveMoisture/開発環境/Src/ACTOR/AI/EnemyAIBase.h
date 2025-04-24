#pragma once
#include <map>
#include <memory>
#include "BehaviorBase.h"
#include "../Enemy/EnemyState.h"
#include "../Enemy/EnemyData.h"
#include "../../Common/Time.h"
#include "../../Common/IsDistance.h"

class ShareData;

class EnemyAIBase : public BehaviorBase
{
protected:

	using DIR_BACK = std::function<Dir3DF(void)>;

public:

	//ラグタイマーの名前
	enum class RAG_NAME
	{
		ATTACK_1_RAG_TIME,		// 攻撃1ラグタイム
		ATTACK_2_RAG_TIME,		// 攻撃2ラグタイム
		ATTACK_3_RAG_TIME,		// 攻撃3ラグタイム
		ATTACK_4_RAG_TIME,		// 攻撃4ラグタイム
		ATTACK_5_RAG_TIME,		// 攻撃5ラグタイム
		ATTACK_6_RAG_TIME,		// 攻撃6ラグタイム
		ATTACK_AWAKE_RAG_TIME,	// 覚醒攻撃ラグタイム
	};

	//コンストラクタ
	EnemyAIBase(EnemyState::STATE& state,
		const DIR_BACK& dirBack,
		const EnemyData::RagTimesData& ragTimes);

	//デストラクタ
	virtual ~EnemyAIBase(void)override;

	// 距離条件判定クラスのセットアップ
	// これをしないと距離判定を正常にできない
	void SetUpIsDistance(Position3DF& enemyPos, Position3DF& playerPos);
	// 条件判定クラスのnullptrチェック
	const bool IsDistanceNull(void);

	// 指定したラグタイムをセット
	void SetUpRagTime(const RAG_NAME& ragTimeName);

	// 攻撃中か
	const bool IsAttack(void)const;

protected:

	// ノードの名前
	enum class NODE_NAME
	{
		PARENT_NODE,				// 親ノード
		ACTION_WOLK_NODE,			// 移動アクションノード
		ACTION_ATTACK_1_NODE,		// 攻撃1アクションノード
		ACTION_ATTACK_2_NODE,		// 攻撃2アクションノード
		ACTION_ATTACK_3_NODE,		// 攻撃3アクションノード
		ACTION_ATTACK_4_NODE,		// 攻撃4アクションノード
		ACTION_ATTACK_5_NODE,		// 攻撃5アクションノード
		ACTION_ATTACK_6_NODE,		// 攻撃6アクションノード
		ACTION_ATTACK_AWAKE_NODE,	// 覚醒攻撃アクションノード
		ACTION_END_SHORT_NODE,		// 攻撃を短い時間停止するノード
		ACTION_END_LONG_NODE,		// 攻撃を長い時間停止するノード
	};


protected:

	using CONDITION = std::function<bool(void)>;
	using NODE_STRINGS = std::map<std::string, NODE_NAME>;
	using CONDITION_STRINGS = std::map<std::string, CONDITION>;

	// ラグタイム
	struct RagTimes
	{
	private:

		using RAG_TIME = std::map<RAG_NAME, Time>;
		using RAG_TIME_MAX = std::map<RAG_NAME, float>;

	public:

		// 攻撃などのラグを計測
		RAG_TIME time;
		// ラグの最大値をセット
		RAG_TIME_MAX time_Max;

	};

	// ノードの名前と列挙型との対応付け
	NODE_STRINGS node_NameStrings_;
	// 遷移条件の名前と関数ポインタとの対応付け
	CONDITION_STRINGS condition_NameStrings_;

private:

	using NAMES = std::vector<std::string>;

protected:
	
	// 現在の状態
	EnemyState::STATE& nowState_;

	// 距離系統の条件判定を行う
	std::unique_ptr<IsDistance> distance_;

	// ラグタイム
	RagTimes ragTimes_;

	// モデルの後ろ方向を取得
	const DIR_BACK dirBack_;

	// 特別性の高い更新
	void UpdateSpecial(void)override;

	// ラグの初期化
	virtual void InitRagTime(const EnemyData::RagTimesData& ragTimes);
	// ラグタイムの計測
	void UpdateRagTime(void);


	//ノード作成
	virtual void CreateNode(void)override;
	//ノードをエッジで結合
	virtual void ConnectEdge(void)override;

	// ノードの名前と列挙型を対応付ける
	void SetUpNodeNameStrings(void);
	// 名前指定で列挙型を返す
	const NODE_NAME& NodeNameStringToNODE_NAME(const std::string& name);
	const std::vector<int> NodeNameStringToNODE_NAMEVec(const NAMES& names);

	// 遷移条件の名前と遷移条件関数を対応付ける
	virtual void SetUpConditionNameString(void);

	// 遷移条件関数を取得
	const CONDITION& ConditionNameStringToCondition(const std::string& name);
	const std::vector<CONDITION>ConditionNameStringToConditionVec(const NAMES& names);


	// 状態遷移
	void ChangeEnemyStateEnd(void);
	void ChangeEnemyStateWalk(void);
	void ChangeEnemyStateAction_1(void);
	void ChangeEnemyStateAction_2(void);
	void ChangeEnemyStateAction_3(void);
	void ChangeEnemyStateAction_4(void);
	void ChangeEnemyStateAction_5(void);
	void ChangeEnemyStateAction_6(void);
	void ChangeEnemyStateAction_Awake(void);
	void ChangeEnemyStateActionEndShort(void);
	void ChangeEnemyStateActionEndLong(void);


	// 遷移条件--------------------------------
	virtual const bool ConditionWalk(void);
	virtual const bool ConditionAttack_1(void);
	virtual const bool ConditionAttack_2(void);
	virtual const bool ConditionAttack_3(void);
	virtual const bool ConditionAttack_4(void);
	virtual const bool ConditionAttack_5(void);
	virtual const bool ConditionAttack_6(void);
	virtual const bool ConditionAttack_Awake(void);
	virtual const bool ConditionNearDistance(void);
	virtual const bool ConditionNearDisBack(void);
	virtual const bool ConditionMediumDistance(void);
	virtual const bool ConditionFarDistance(void);

	//遷移条件::ラグによる攻撃1アニメーションの起動失敗条件
	const bool ConditionEndAttack_1_Rag(void)const;
	//遷移条件::ラグによる攻撃2アニメーションの起動失敗条件
	const bool ConditionEndAttack_2_Rag(void)const;
	//遷移条件::ラグによる攻撃3アニメーションの起動失敗条件
	const bool ConditionEndAttack_3_Rag(void)const;
	//遷移条件::ラグによる攻撃4アニメーションの起動失敗条件
	const bool ConditionEndAttack_4_Rag(void)const;
	//遷移条件::ラグによる攻撃5アニメーションの起動失敗条件
	const bool ConditionEndAttack_5_Rag(void)const;
	//遷移条件::ラグによる攻撃6アニメーションの起動失敗条件
	const bool ConditionEndAttack_6_Rag(void)const;
	// 遷移条件::ラグによる覚醒攻撃アニメーションの起動失敗条件
	const bool ConditionEndAttack_Awake_Rag(void)const;
	// ----------------------------------------

};