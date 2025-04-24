#pragma once
#include <cassert>
#include<functional>
#include<vector>

class Node;

class Edge
{
public:

	//�R���X�g���N�^
	Edge(int from, 
		const std::vector<int>&end,
		const std::vector<std::function<bool(void)>>&condition,
		const std::vector<float>& weight) :

		fromIdx_(from),
		endIdxs_(end),
		weight_(weight),
		condition_(condition)
	{
	}


	//�f�X�g���N�^
	~Edge(void)
	{
	}

	//�R�X�g���擾
	const float GetCost(int number)const
	{
		assert(number < weight_.size() || !"�o�^����Edge�̃R�X�g���ȏ�͎w��ł��܂���");
		assert(0 <= number || !"-1�͋��e�ł��܂���");

		return weight_[number];
	}


	//�t�����m�[�h�̃C���f�b�N�X���擾
	const int GetFromIndex(void)const
	{
		return fromIdx_;
	}

	//�G���h�m�[�h�̓o�^�����擾
	const int GetEndNodeNum(void)const
	{
		return static_cast<int>(endIdxs_.size());
	}

	//�w�肷��G���h�m�[�h�̃C���f�b�N�X���擾
	const int GetEndIndex(int number)const
	{
		assert(number < endIdxs_.size() || !"�o�^����Edge�̃C���f�b�N�X���ȏ�͎w��ł��܂���");
		assert(0 <= number || !"-1�͋��e�ł��܂���");

		return endIdxs_[number];
	}


	//�J�ڏ�����B��������
	const bool IsCondition(int number)const
	{
		assert(number < condition_.size() || !"�o�^����Edge�̃R���f�B�V�������ȏ�͎w��ł��܂���");
		assert(0 <= number || !"-1�͋��e�ł��܂���");

		return condition_[number]();
	}

private:

	//�J�ڏ����֐��|�C���^
	using ConditionFunc = std::vector<std::function<bool(void)>>;

private:

	// �J�ڏ���
	ConditionFunc condition_;

	//�t�����m�[�h�̃C���f�b�N�X
	int fromIdx_;
	//�G���h�m�[�h�̃C���f�b�N�X
	std::vector<int> endIdxs_;

	//�G�b�W�̏d��
	std::vector<float> weight_;

};