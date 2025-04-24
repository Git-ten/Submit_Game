#include "NodeBase.h"

NodeBase::NodeBase(int index)
{
	NodeIndex_ = index;
	IsEndNode_ = false;
}

NodeBase::~NodeBase()
{
}

void NodeBase::Update()
{
}

int NodeBase::GetIndex()const
{
	return NodeIndex_;
}

void NodeBase::SetIndex(int index)
{
	NodeIndex_ = index;
}

int NodeBase::IsCondition()const
{
	return -1;
}

bool NodeBase::IsEndAction()const
{
	return true;
}

void NodeBase::SetIsEndAction(bool flg)
{
}

bool NodeBase::IsEndNode()const
{
	return IsEndNode_;
}
