#include"NodeBase.h"
#include "ConditionNode.h"

ConditionNode::ConditionNode(int index, std::function<int(void)> condition) : NodeBase(index)
{
	condition_ = condition;
	IsEndNode_ = false;//�����m�[�h�͕K���I�[�m�[�h�ł͂Ȃ�
}

ConditionNode::~ConditionNode()
{
}

int ConditionNode::IsCondition()const
{
	return condition_();
}