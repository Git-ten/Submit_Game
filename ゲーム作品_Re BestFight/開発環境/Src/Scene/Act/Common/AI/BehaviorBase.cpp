#include"Node/NodeBase.h"
#include"Node/ActionNode.h"
#include"Node/ConditionNode.h"
#include"Node/Edge.h"
#include "BehaviorBase.h"

BehaviorBase::BehaviorBase()
{
	nowIndex_ = NONE_INDEX;
	nowCost_ = NONE_COST;
}

BehaviorBase::~BehaviorBase()
{
}

void BehaviorBase::Init()
{
	InitNode();
	InitEdge();
	nowIndex_ = -1;
}

void BehaviorBase::DebugDraw()
{
}

void BehaviorBase::SetNowIndex(int index)
{
	nowIndex_ = index;
	nowCost_ = NONE_COST;
}

int BehaviorBase::GetNowIndex() const
{
	return nowIndex_;
}

void BehaviorBase::InitNode()
{
	nodes_.clear();
	//ノード生成
	CreateNode();
}

void BehaviorBase::InitEdge()
{
	edges_.clear();
	//接続処理
	ConnectNode();
}

bool BehaviorBase::CheckNode(int index)
{
	//チェック
	//同じキーは新しく入力できない
	for (auto& node : nodes_)
	{
		if (node->GetIndex() == index)
		{
			return false;
		}
	}

	return true;
}

bool BehaviorBase::CheckEdgeAll()
{
	for (auto& edge : edges_)
	{
		//エッジが登録されていない
		if (edge == nullptr)
		{
			return false;
		}
	}
	return true;
}

void BehaviorBase::AddConditionNode(int index, std::function<int(void)> func)
{
	nodes_.push_back(std::make_unique<ConditionNode>(index, func));
}

void BehaviorBase::AddActionNode(int index, std::function<void(void)> func)
{
	nodes_.push_back(std::make_unique<ActionNode>(index, func));
}

void BehaviorBase::AddConnectEdge(int fromIdx, int toIdx, float cost)
{
	edges_.push_back(std::make_unique<Edge>(fromIdx, toIdx, cost));
}

void BehaviorBase::CreateNode()
{
	/*int cnt = INT_INDEX(INDEX_CON_MOVE_UP);
	AddConditionNode(cnt++, [&]() {return ConditionMoveUp(); });*/
}

void BehaviorBase::ConnectNode()
{
	/*int cnt = 0;
	AddConnectEdge(node[idx]->GetIndex(), node[otherIdx]->GetIndex(), cost);*/
}

int BehaviorBase::SearchNodeIdx(int index)
{
	int cnt = 0;
	for (const auto& node : nodes_)
	{
		if (node->GetIndex() == index)
		{
			return cnt;
		}
		cnt++;
	}
	return -1;
}

int BehaviorBase::SearchEdgeIdx(int fromIdx, int toIdx)
{
	int cnt = 0;
	for (const auto& edge : edges_)
	{
		if (edge->GetPrevIndex() == fromIdx && edge->GetNextIndex() == toIdx)
		{
			return cnt;
		}
		cnt++;
	}
	return -1;
}

void BehaviorBase::EndNodeAction(int num)
{
	//アクションを終了
	nodes_[num]->SetIsEndAction(true);
	nowIndex_ = NONE_INDEX;
	nowCost_ = NONE_COST;
}

int BehaviorBase::ConditionBranch()
{
	return -1;
}

void BehaviorBase::NodeTransition(int index, int& Idx)
{
	int nSearch = SearchNodeIdx(index);
	if (nSearch >= 0)
	{
		int nextIdx = nodes_[nSearch]->IsCondition();

		if (nextIdx != NONE_INDEX)
		{
			int eSearch = SearchEdgeIdx(index, nextIdx);
			if (eSearch >= 0)
			{
				if (nowCost_ < edges_[eSearch]->GetCost())
				{
					nowCost_ = edges_[eSearch]->GetCost();
					Idx = nextIdx;
				}
			}
		}
	}
}