#include <cassert>
#include"Node/Node.h"
#include"Node/Edge.h"
#include "BehaviorBase.h"

#pragma region �萔�錾

//�R�X�g����(���݃R�X�g�̏������p)
const float NONE_WEIGHT = 0.0f;
//�C���f�b�N�X�Ȃ�
const int NONE_INDEX = -1;
//�C���f�b�N�X�̏������l(parent�̃C���f�b�N�X)
const int INIT_PARENT_INDEX = 0;

//AI�̎v�l��~����
//������
const float STOP_AI_TIME_LONG = 1.5f;
//�Z����
const float STOP_AI_TIME_SHORT = 0.5f;

#pragma endregion

BehaviorBase::BehaviorBase(void):
	nowIndex_(NONE_INDEX),
	isEndAIAction_(false),
	lockThinkTime_(Time()),
	lockTime_(0.0f),
	isPerfectLock_(false)
{
}

BehaviorBase::~BehaviorBase(void)
{
}

void BehaviorBase::Init(void)
{
	InitNode();
	InitEdge();
	nowIndex_ = INIT_PARENT_INDEX;
}

void BehaviorBase::Update(void)
{
	// ���b�N���Ԃ��v��
	LockTimeProcess();

	// ���b�N���łȂ���Ύv�l����
	if (!IsLockThink() && !IsPerfectLock())NodeTransition();


	// �A�N�V�����̓��b�N���ԂɊ֌W�Ȃ����s
	// �A�N�V���������s
	NodeAction();

	UpdateSpecial();
}

void BehaviorBase::InitNode(void)
{
	nodes_.clear();

	//�m�[�h����
	CreateNode();
}

void BehaviorBase::InitEdge(void)
{
	edges_.clear();

	//�ڑ�����
	ConnectEdge();
}

void BehaviorBase::AddNode(int index, std::function<void(void)> func)
{
	nodes_.emplace(index, std::make_shared<Node>(index, func));
}

void BehaviorBase::AddEdge(const std::string& edgeName,
	int fromIndex,
	const std::vector<int>& end,
	const std::vector<std::function<bool(void)>>& condition,
	const std::vector<float>& weight)
{
	edges_.emplace(edgeName, std::make_unique<Edge>(fromIndex, end, condition, weight));
}

void BehaviorBase::LockThinkShortTime(void)
{
	LockThink(STOP_AI_TIME_SHORT);
}

void BehaviorBase::LockThinkLongTime(void)
{
	LockThink(STOP_AI_TIME_LONG);
}

void BehaviorBase::PerfectLock(void)
{
	isPerfectLock_ = true;
	nowIndex_ = NONE_INDEX;
}

void BehaviorBase::UnPerfectLock(void)
{
	isPerfectLock_ = false;
	nowIndex_ = INIT_PARENT_INDEX;
}

void BehaviorBase::LockThink(const float time)
{
	lockTime_ = time;
	lockThinkTime_.InitTotalNowTime();
}

bool BehaviorBase::IsPerfectLock(void)
{
	return isPerfectLock_;
}

bool BehaviorBase::IsLockThink(void) const
{
	return lockThinkTime_.GetTotalNowTime() < lockTime_;
}

void BehaviorBase::LockTimeProcess(void)
{
	lockThinkTime_.UpdateTime();
}

void BehaviorBase::EndAIAction(void)
{
	isEndAIAction_ = true;
	nowIndex_ = INIT_PARENT_INDEX;
}

bool BehaviorBase::IsEndAIAction(void) const
{
	return isEndAIAction_;
}

int BehaviorBase::TransitionNextNode(int fromIdx)
{
	int idx = NONE_INDEX;
	float nowWeight = NONE_WEIGHT;

	int endNodeSize = 0;

	for (const auto& edge : edges_)
	{
		if (edge.second->GetFromIndex() != fromIdx)continue;
		
		//�ڑ�����Ă��邷�ׂẴm�[�h������
		endNodeSize = edge.second->GetEndNodeNum();
		for (int ii = 0; ii < endNodeSize; ii++)
		{
			if (!edge.second->IsCondition(ii))continue;

			//�R�X�g���傫���قǗD��x������
			//�R�X�g�������Ȃ�A��Ɋi�[����Ă��鏇
			if (nowWeight <= edge.second->GetCost(ii))
			{
				nowWeight = edge.second->GetCost(ii);
				idx = edge.second->GetEndIndex(ii);
			}
		}

		// �G�b�W�����������Ȃ�I��
		break;
	}

	return idx;
}

void BehaviorBase::NodeTransition(void)
{
	//�T������������
	bool isSearch = false;
	//�J�ڌ�̃C���f�b�N�X
	int nextIndex = NONE_INDEX;

	while (!isSearch)
	{
		nextIndex = TransitionNextNode(nowIndex_);

		//���̃m�[�h�ɑJ�ڂ��Ȃ�����
		// ���[�v�̏I��
		if (nextIndex == NONE_INDEX)isSearch = true;

		// ���̃C���f�b�N�X�ɑJ��
		else
		{
			nowIndex_ = nextIndex;
			// AI�̃A�N�V�����I����������
			isEndAIAction_ = false;
		}
	}
}

void BehaviorBase::NodeAction(void)
{
	if (nowIndex_ == NONE_INDEX)return;

	const auto& ite = nodes_.find(nowIndex_);
	assert(ite != nodes_.end() || !"���݂��Ȃ��m�[�h�C���f�b�N�X�͎w��ł��܂���");

	// �A�N�V����
	nodes_.at(nowIndex_)->NodeAction();
}