#pragma once
#include<functional>

class Node
{
private:

	//�A�N�V�����֐��|�C���^
	//�X�V�p�֐��|�C���^
	using ActionFunc = std::function<void(void)>;

public:

	//�R���X�g���N�^
	Node(int index, const ActionFunc& action) :
		nodeIndex_(index),
		action_(action)
	{

	}

	//�f�X�g���N�^
	~Node(void)
	{

	}


	//�p����ŋ��ʂ��Ďg�p����֐�
	const int GetIndex(void)const
	{
		return nodeIndex_;
	}

	//�A�N�V��������
	void NodeAction(void)
	{
		//�A�N�V�������o�^����Ă��Ȃ���Ή������Ȃ�
		if (action_ == nullptr)return;

		action_();
	}

private:

	//�X�V�֐��|�C���^
	const ActionFunc action_;

	//���g�̃m�[�h�ԍ�
	const int nodeIndex_;

};