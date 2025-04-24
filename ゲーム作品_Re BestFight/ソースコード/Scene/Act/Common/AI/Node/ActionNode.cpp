#include"NodeBase.h"
#include "ActionNode.h"

ActionNode::ActionNode(int index, std::function<void(void)> action) : NodeBase(index)
{
	action_ = action;

	//アクションノードは必ず終端
	//ただし、アクションとして別のノードに分岐する処理を
	//することはある
	IsEndNode_ = true;

	IsEndAction_ = false;
}

ActionNode::~ActionNode()
{
}

void ActionNode::Update()
{
	action_();
}

bool ActionNode::IsEndAction() const
{
	return IsEndAction_;
}

void ActionNode::SetIsEndAction(bool flg)
{
	IsEndAction_ = flg;
}