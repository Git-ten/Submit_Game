#pragma once
#include<functional>
#include "NodeBase.h"
class ConditionNode : public NodeBase
{
public:

	//�R���X�g���N�^
	ConditionNode(int index, std::function<int(void)> condition);
	//�f�X�g���N�^
	~ConditionNode(void)override;

	//�����𖞂�������
	int IsCondition(void)const override;

private:

	using ConditionFunc = std::function<int(void)>;

	//�����֐����i�[
	//���s�̓m�[�h�̍쐬��
	ConditionFunc condition_;

};