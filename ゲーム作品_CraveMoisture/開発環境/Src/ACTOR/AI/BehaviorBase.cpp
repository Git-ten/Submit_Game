#include <cassert>
#include"Node/Node.h"
#include"Node/Edge.h"
#include "BehaviorBase.h"

#pragma region 定数宣言

//コスト無し(現在コストの初期化用)
const float NONE_WEIGHT = 0.0f;
//インデックスなし
const int NONE_INDEX = -1;
//インデックスの初期化値(parentのインデックス)
const int INIT_PARENT_INDEX = 0;

//AIの思考停止時間
//長時間
const float STOP_AI_TIME_LONG = 1.5f;
//短時間
const float STOP_AI_TIME_SHORT = 0.5f;

#pragma endregion

BehaviorBase::BehaviorBase(void):
	nowIndex_(NONE_INDEX),
	isEndAIAction_(false),
	lockThinkTime_(Time()),
	lockTime_(0.0f),
	isPerfectLock_(false)
{
}

BehaviorBase::~BehaviorBase(void)
{
}

void BehaviorBase::Init(void)
{
	InitNode();
	InitEdge();
	nowIndex_ = INIT_PARENT_INDEX;
}

void BehaviorBase::Update(void)
{
	// ロック時間を計測
	LockTimeProcess();

	// ロック中でなければ思考する
	if (!IsLockThink() && !IsPerfectLock())NodeTransition();


	// アクションはロック時間に関係なく実行
	// アクションを実行
	NodeAction();

	UpdateSpecial();
}

void BehaviorBase::InitNode(void)
{
	nodes_.clear();

	//ノード生成
	CreateNode();
}

void BehaviorBase::InitEdge(void)
{
	edges_.clear();

	//接続処理
	ConnectEdge();
}

void BehaviorBase::AddNode(int index, std::function<void(void)> func)
{
	nodes_.emplace(index, std::make_shared<Node>(index, func));
}

void BehaviorBase::AddEdge(const std::string& edgeName,
	int fromIndex,
	const std::vector<int>& end,
	const std::vector<std::function<bool(void)>>& condition,
	const std::vector<float>& weight)
{
	edges_.emplace(edgeName, std::make_unique<Edge>(fromIndex, end, condition, weight));
}

void BehaviorBase::LockThinkShortTime(void)
{
	LockThink(STOP_AI_TIME_SHORT);
}

void BehaviorBase::LockThinkLongTime(void)
{
	LockThink(STOP_AI_TIME_LONG);
}

void BehaviorBase::PerfectLock(void)
{
	isPerfectLock_ = true;
	nowIndex_ = NONE_INDEX;
}

void BehaviorBase::UnPerfectLock(void)
{
	isPerfectLock_ = false;
	nowIndex_ = INIT_PARENT_INDEX;
}

void BehaviorBase::LockThink(const float time)
{
	lockTime_ = time;
	lockThinkTime_.InitTotalNowTime();
}

bool BehaviorBase::IsPerfectLock(void)
{
	return isPerfectLock_;
}

bool BehaviorBase::IsLockThink(void) const
{
	return lockThinkTime_.GetTotalNowTime() < lockTime_;
}

void BehaviorBase::LockTimeProcess(void)
{
	lockThinkTime_.UpdateTime();
}

void BehaviorBase::EndAIAction(void)
{
	isEndAIAction_ = true;
	nowIndex_ = INIT_PARENT_INDEX;
}

bool BehaviorBase::IsEndAIAction(void) const
{
	return isEndAIAction_;
}

int BehaviorBase::TransitionNextNode(int fromIdx)
{
	int idx = NONE_INDEX;
	float nowWeight = NONE_WEIGHT;

	int endNodeSize = 0;

	for (const auto& edge : edges_)
	{
		if (edge.second->GetFromIndex() != fromIdx)continue;
		
		//接続されているすべてのノードを検索
		endNodeSize = edge.second->GetEndNodeNum();
		for (int ii = 0; ii < endNodeSize; ii++)
		{
			if (!edge.second->IsCondition(ii))continue;

			//コストが大きいほど優先度が高い
			//コストが同じなら、後に格納されている順
			if (nowWeight <= edge.second->GetCost(ii))
			{
				nowWeight = edge.second->GetCost(ii);
				idx = edge.second->GetEndIndex(ii);
			}
		}

		// エッジが見つかったなら終了
		break;
	}

	return idx;
}

void BehaviorBase::NodeTransition(void)
{
	//探索完了したか
	bool isSearch = false;
	//遷移後のインデックス
	int nextIndex = NONE_INDEX;

	while (!isSearch)
	{
		nextIndex = TransitionNextNode(nowIndex_);

		//次のノードに遷移しなかった
		// ループの終了
		if (nextIndex == NONE_INDEX)isSearch = true;

		// 次のインデックスに遷移
		else
		{
			nowIndex_ = nextIndex;
			// AIのアクション終了を取り消す
			isEndAIAction_ = false;
		}
	}
}

void BehaviorBase::NodeAction(void)
{
	if (nowIndex_ == NONE_INDEX)return;

	const auto& ite = nodes_.find(nowIndex_);
	assert(ite != nodes_.end() || !"存在しないノードインデックスは指定できません");

	// アクション
	nodes_.at(nowIndex_)->NodeAction();
}