#include"NodeBase.h"
#include "ConditionNode.h"

ConditionNode::ConditionNode(int index, std::function<int(void)> condition) : NodeBase(index)
{
	condition_ = condition;
	IsEndNode_ = false;//条件ノードは必ず終端ノードではない
}

ConditionNode::~ConditionNode()
{
}

int ConditionNode::IsCondition()const
{
	return condition_();
}