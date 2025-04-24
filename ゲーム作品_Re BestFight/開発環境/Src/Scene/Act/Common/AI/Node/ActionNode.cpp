#include"NodeBase.h"
#include "ActionNode.h"

ActionNode::ActionNode(int index, std::function<void(void)> action) : NodeBase(index)
{
	action_ = action;

	//�A�N�V�����m�[�h�͕K���I�[
	//�������A�A�N�V�����Ƃ��ĕʂ̃m�[�h�ɕ��򂷂鏈����
	//���邱�Ƃ͂���
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