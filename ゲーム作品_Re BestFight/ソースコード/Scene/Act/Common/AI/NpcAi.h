#pragma once
#include "BehaviorBase.h"

class ActorBase;
class ActorAi;

class NpcAi : public BehaviorBase
{
public:

	//コンストラクタ
	NpcAi(std::weak_ptr<ActorAi>targetAi, std::weak_ptr<ActorBase>actor);

	//デストラクタ
	~NpcAi(void)override;

	//更新
	void Update(void)override;

	void DebugDraw(void)override;

private:

	//インデックス名
	enum INDEX
	{
		//作戦選択条件
		INDEX_CON_OPERATION_MOVE = 1,
		INDEX_CON_OPERATION_ESCAPE,
		INDEX_CON_OPERATION_ATTACK,

		//移動条件
		INDEX_CON_MOVE,
		INDEX_CON_MOVE_BRANCH,

		//移動処理
		INDEX_ACT_MOVE_FAR,
		INDEX_ACT_MOVE_MEDIUM,

		//回避条件
		INDEX_CON_ESCAPE,
		INDEX_CON_ESCAPE_BRANCH,

		//回避処理
		INDEX_ACT_ESCAPE_AVOID,
		INDEX_ACT_ESCAPE_RUN,
		INDEX_ACT_ESCAPE_DEFENCE,

		//攻撃条件
		INDEX_CON_ATTACK,
		INDEX_CON_ATTACK_BRANCH,

		//攻撃処理
		INDEX_ACT_ATTACK_WEEK,
		INDEX_ACT_ATTACK_STRONG,
		
		//必殺技処理
		INDEX_ACT_UNLEASHSKILL,
	};

	//使用するAI
	std::weak_ptr<ActorAi>targetAi_;
	//対象アクター
	std::weak_ptr<ActorBase>actor_;



	//内部でのみ使用する関数
	void CreateNode(void)override;
	void ConnectNode(void)override;
	int ConditionBranch(void)override;
	void ConditionStartNode(int& idx);

	//ノードに登録する用の関数
	//作戦選択条件
	int ConditionOperateMove(void);
	int ConditionOperateEscape(void);
	int ConditionOperateAttack(void);

	//移動条件
	int ConditionMove(void);
	int ConditionMoveBranch(void);

	//回避条件
	int ConditionEscape(void);
	int ConditionEscapeBranch(void);

	//攻撃条件
	int ConditionAttack(void);
	int ConditionAttackBranch(void);

	//アクションの実態
	//移動アクション
	void ActionMoveFar(void);
	void ActionMoveMedium(void);
	
	//回避アクション
	void ActionEscapeAvoid(void);
	void ActionEscapeRun(void);
	void ActionEscapeDefence(void);

	//攻撃アクション
	void ActionAttackWeek(void);
	void ActionAttackStrong(void);

	//必殺技アクション
	void ActionUnleashSkill(void);
};