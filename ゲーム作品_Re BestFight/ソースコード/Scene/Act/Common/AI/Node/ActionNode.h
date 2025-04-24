#pragma once
#include<functional>
#include "NodeBase.h"
class ActionNode : public NodeBase
{
public:

	//�R���X�g���N�^
	ActionNode(int index, std::function<void(void)> action);
	//�f�X�g���N�^
	~ActionNode(void)override;

	//�X�V
	void Update(void)override;

	//�A�N�V�������I�����������Ă��邩
	bool IsEndAction()const override;

	//�A�N�V�������I�����������Z�b�g
	void SetIsEndAction(bool flg)override;

private:

	using ActionFunc = std::function<void(void)>;

	//�A�N�V�����֐����i�[
	//���s�̓m�[�h�̍쐬��
	ActionFunc action_;

	//���������ۂ�
	bool IsEndAction_;

};