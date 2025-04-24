#include"../../../../Utility/MyUtility.h"
#include"Node/NodeBase.h"
#include"Node/ActionNode.h"
#include"Node/ConditionNode.h"
#include"Node/Edge.h"
#include"../../ActorBase.h"
#include"../../Common/Userinterface.h"
#include"../../Common/LevelUpManager.h"
#include"../../Common/AnimationController.h"
#include"../../../Common/Collider.h"
#include"../../../../Manager/SoundManager.h"
#include"../../../../Manager/InputManager.h"
#include"../../../Camera.h"
#include<DxLib.h>
#include<cmath>
#include "ActorAi.h"

static InputManager& input = InputManager::GetInstance();

ActorAi::ActorAi(std::weak_ptr<ActorBase> actor)
{
	frameSpeed_ = DEFAULT_FRAME_SPEED;
	ChangeMode(MODE::MANUAL);
	actor_ = actor;
	nowIndex_ = NONE_INDEX;
	nowCost_ = NONE_COST;
	//�����蔻��t���O
	colliderFlag_ = false;
	//�ړ����Ă��邩
	moveFlag_ = false;
	//�ړ�����
	moveDir_ = MyUtility::VECTOR_ZERO;
	//�R�}���h�F��U���̉񐔂��L�^
	atackWeakCnt_ = 0;
	atackStrongCnt_ = 0;
	//�R�}���h�������Ă��邩
	comandFlag_ = false;
	//������
	avoidDir_ = MyUtility::VECTOR_ZERO;
	//�O��̉�����
	prevAvoidDir_ = MyUtility::VECTOR_ZERO;
	//�W�����v���Ă��邩
	jumpFlag_ = false;
	//�W���X�g����Ɏg��
	damageFlag_ = false;

	InitTime();//���Ԃ̏�����
}

ActorAi::~ActorAi()
{

}

void ActorAi::Update()
{
	if (timeCounter_.at(TIME::FRAME_TIME) <= 0.0f)
	{
		updateFunc_();

		//�ϐ�������
		SetTime(TIME::FRAME_TIME, DEFAULT_FRAME_TIME);
		actor_.lock()->SetUpdateFlag(true);
	}
	else
	{
		actor_.lock()->SetUpdateFlag(false);
	}

	UpdateTime();
}

void ActorAi::SetTime(const TIME& type, float t)
{
	const auto& ite = timeCounter_.find(type);
	if (ite != timeCounter_.end())
	{
		timeCounter_.at(type) = t;
	}
}

void ActorAi::SetFrameSpeed(float speed)
{
	frameSpeed_ = speed;
}

void ActorAi::SetFrameDefaultSpeed()
{
	frameSpeed_ = DEFAULT_FRAME_SPEED;
}

bool ActorAi::GetDamageFlag() const
{
	return damageFlag_;
}

void ActorAi::SetDamageFlag(bool flg)
{
	damageFlag_ = flg;
}

void ActorAi::ResetAttackCount()
{
	atackWeakCnt_ = 0;
	atackStrongCnt_ = 0;
}

void ActorAi::ChangeMode(const MODE& mode)
{
	mode_ = mode;
	switch (mode_)
	{
	case MODE::AUTO:
		updateFunc_ = [&]() {UpdateAuto(); };
		break;
	case MODE::MANUAL:
		updateFunc_ = [&]() {UpdateManual(); };
		break;
	default:
		break;
	}
}

ActorAi::MODE ActorAi::GetMode() const
{
	return mode_;
}

bool ActorAi::CollFlag(void)
{
	return colliderFlag_;
}

void ActorAi::InitTime()
{
	timeCounter_.clear();
	timeCounter_.emplace(TIME::FRAME_TIME, 0.0f);
	timeCounter_.emplace(TIME::ATTACK_RAG, 0.0f);
	timeCounter_.emplace(TIME::AVOID_TIME, 0.0f);
	timeCounter_.emplace(TIME::QUICK_TIME, 0.0f);
	timeCounter_.emplace(TIME::QUICK_RAG, 0.0f);
	timeCounter_.emplace(TIME::JUST_RECEP_TIME, 0.0f);
	timeCounter_.emplace(TIME::JUST_CHANCE_TIME, 0.0f);
	timeCounter_.emplace(TIME::GURD_RAG, 0.0f);
	timeCounter_.emplace(TIME::COUNTER_RAG, 0.0f);
}

void ActorAi::UpdateTime()
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	for (auto& t : timeCounter_)
	{
		if (t.second > 0.0f)
		{
			t.second -= delta * frameSpeed_;
		}
		else
		{
			//������
			t.second = 0.0f;
		}
	}
}

void ActorAi::UpdateAuto()
{
	if (nowIndex_ != NONE_INDEX)
	{
		NodeTransition(nowIndex_, nowIndex_);

		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			//�I�[�m�[�h�̏ꍇ�̂ݏ���
			if (nodes_[nSearch]->IsEndNode())
			{
				//�A�N�V�������I��
				if (nodes_[nSearch]->IsEndAction())
				{
					nodes_[nSearch]->SetIsEndAction(false);
				}
				else
				{
					nodes_[nSearch]->Update();
				}
			}
		}
	}

	ParamInitStaminaRecove();
}

void ActorAi::UpdateManual()
{
	//���s�\�ȃm�[�h�̒T��
	int sCounter = 0;
	while (sCounter < MAX_NODE_SEARCH)
	{
		nowIndex_ = ConditionBranch();
		if (ParamInitStaminaRecove())break;

		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			//�I�[�m�[�h�̏ꍇ�̂ݏ���
			if (nodes_[nSearch]->IsEndNode())
			{
				//�A�N�V�������I��
				if (nodes_[nSearch]->IsEndAction())
				{
					nodes_[nSearch]->SetIsEndAction(false);
				}
				else
				{
					nodes_[nSearch]->Update();
				}
				break;
			}
		}

		sCounter++;
	}
}

void ActorAi::CreateNode()
{
	//�ړ��n�m�[�h-------------------------------
	//�蓮
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_UP), [&]() {return ConditionMoveUp(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_DOWN), [&]() {return ConditionMoveDown(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_LEFT), [&]() {return ConditionMoveLeft(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_RIGHT), [&]() {return ConditionMoveRight(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_LEFTUP), [&]() {return ConditionMoveLeftUp(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_RIGHTUP), [&]() {return ConditionMoveRightUp(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_LEFTDOWN), [&]() {return ConditionMoveLeftDown(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN), [&]() {return ConditionMoveRightDown(); });

	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_UP), [&]() {ActionMoveUp(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_DOWN), [&]() {ActionMoveDown(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_LEFT), [&]() {ActionMoveLeft(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_RIGHT), [&]() {ActionMoveRight(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_LEFTUP), [&]() {ActionMoveLeftUp(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_RIGHTUP), [&]() {ActionMoveRightUp(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN), [&]() {ActionMoveLeftDown(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN), [&]() {ActionMoveRightDown(); });


	//����
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_MOVE_FAR), [&]() {return ConditionAutoMoveFar(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_MOVE_MEDIUM), [&]() {return ConditionAutoMoveMedium(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_DEFENCE), [&]() {return ConditionAutoDefence(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_AVOID), [&]() {return ConditionAutoAvoid(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_RUN), [&]() {return ConditionAutoRun(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_WEEK), [&]() {return ConditionAutoWeek(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_STRONG), [&]() {return ConditionAutoStrong(); });

	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_MOVE_FAR), [&]() {ActionAutoMoveFar(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM), [&]() {ActionAutoMoveMedium(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_DEFENCE), [&]() {ActionAutoDefence(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_AVOID), [&]() {ActionAutoAvoid(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_RUN), [&]() {ActionAutoRun(); });

	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_COUNTER), [&]() {ActionAutoCounterAttack(); });
	//--------------------------------------------

	//�R�}���h�n�m�[�h----------------------------
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_WEEK), [&]() {return ConditionComandWeek(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH), [&]() {return ConditionComandWeekBranch(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_STRONG), [&]() {return ConditionComandStrong(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH), [&]() {return ConditionComandStrongBranch(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_AVOID), [&]() {return ConditionComandAvoid(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_QUICKMOVE), [&]() {return ConditionComandQuickMove(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_JUMP), [&]() {return ConditionComandJump(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_FALLATTACK), [&]() {return ConditionComandFallAttack(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_DEFENCE), [&]() {return ConditionComandDefence(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_UNLEASHSKILL), [&]() {return ConditionComandUnleashSkill(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK), [&]() {return ConditionComandCounterAttack(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_1), [&]() {ActionComandWeek_1(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_2), [&]() {ActionComandWeek_2(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_3), [&]() {ActionComandWeek_3(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_4), [&]() {ActionComandWeek_4(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_5), [&]() {ActionComandWeek_5(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_6), [&]() {ActionComandWeek_6(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_STRONG_1), [&]() {ActionComandStrong_1(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_STRONG_2), [&]() {ActionComandStrong_2(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_STRONG_3), [&]() {ActionComandStrong_3(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_AVOID), [&]() {ActionComandAvoid(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE), [&]() {ActionComandQuickMove(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_JUMP), [&]() {ActionComandJump(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_FALLATTACK), [&]() {ActionComandFallAttack(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_DEFENCE), [&]() {ActionComandDefence(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_UNLEASHSKILL), [&]() {ActionComandUnleashSkill(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK), [&]() {ActionComandCounterAttack(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_FAIL_COUNTERATTACK), [&]() {ActionComandFailCounterAttack(); });
	//--------------------------------------------
}

void ActorAi::ConnectNode()
{
	//����
	//�������ړ������[���������ړ�����
	int from = INT_INDEX(INDEX_CON_AUTO_MOVE_FAR);
	int to = INT_INDEX(INDEX_ACT_AUTO_MOVE_FAR);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�������ړ������[���������ړ�����
	from = INT_INDEX(INDEX_CON_AUTO_MOVE_MEDIUM);
	to = INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�K�[�h�����[���K�[�h����
	from = INT_INDEX(INDEX_CON_AUTO_DEFENCE);
	to = INT_INDEX(INDEX_ACT_AUTO_DEFENCE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//��������[���������
	from = INT_INDEX(INDEX_CON_AUTO_AVOID);
	to = INT_INDEX(INDEX_ACT_AUTO_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//��������[�����鏈��
	from = INT_INDEX(INDEX_CON_AUTO_RUN);
	to = INT_INDEX(INDEX_ACT_AUTO_RUN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//������U�������[��������U������
	from = INT_INDEX(INDEX_CON_AUTO_WEEK);
	to = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//������U�������[�������W�����v����
	from = INT_INDEX(INDEX_CON_AUTO_WEEK);
	to = INT_INDEX(INDEX_ACT_COMAND_JUMP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�������U�������[���������U������
	from = INT_INDEX(INDEX_CON_AUTO_STRONG);
	to = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�蓮
	//�ړ��������A�N�V�����m�[�h�̃G�b�W
	//������ړ������[��������ړ�
	from = INT_INDEX(INDEX_CON_MOVE_UP);
	to = INT_INDEX(INDEX_ACT_MOVE_UP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�������ړ������[���������ړ�
	from = INT_INDEX(INDEX_CON_MOVE_DOWN);
	to = INT_INDEX(INDEX_ACT_MOVE_DOWN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�������ړ������[��������
	from = INT_INDEX(INDEX_CON_MOVE_LEFT);
	to = INT_INDEX(INDEX_ACT_MOVE_LEFT);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//�E�����ړ������[���E����
	from = INT_INDEX(INDEX_CON_MOVE_RIGHT);
	to = INT_INDEX(INDEX_ACT_MOVE_RIGHT);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//��������ړ������[���������
	from = INT_INDEX(INDEX_CON_MOVE_LEFTUP);
	to = INT_INDEX(INDEX_ACT_MOVE_LEFTUP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);

	//�E������ړ������[���E�����
	from = INT_INDEX(INDEX_CON_MOVE_RIGHTUP);
	to = INT_INDEX(INDEX_ACT_MOVE_RIGHTUP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);

	//���������ړ������[����������
	from = INT_INDEX(INDEX_CON_MOVE_LEFTDOWN);
	to = INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);

	//�E�������ړ������[���E������
	from = INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN);
	to = INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);


	//�R�}���h���́��A�N�V�����m�[�h�̃G�b�W
	//��U���R�}���h���͏����[����U������
	from = INT_INDEX(INDEX_CON_COMAND_WEEK);
	to = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.0f);

	//��U������[����U��1
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_1);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//��U������[����U��2
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_2);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.2f);

	//��U������[����U��3
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_3);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.3f);

	//��U������[����U��4
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_4);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.4f);

	//��U������[����U��5
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_5);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.5f);

	//��U������[����U��6
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_6);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.6f);

	//���U���R�}���h���͏����[�����U������
	from = INT_INDEX(INDEX_CON_COMAND_STRONG);
	to = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.0f);

	//���U������[�����U��1
	from = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_STRONG_1);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//���U������[�����U��2
	from = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_STRONG_2);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.2f);

	//���U������[�����U��3
	from = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_STRONG_3);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.3f);

	//����R�}���h���͏����[�����
	from = INT_INDEX(INDEX_CON_COMAND_AVOID);
	to = INT_INDEX(INDEX_ACT_COMAND_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.5f);

	//�����ړ��R�}���h���͏����[�������ړ�
	from = INT_INDEX(INDEX_CON_COMAND_QUICKMOVE);
	to = INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.6f);

	//�W�����v�R�}���h���͏����[���W�����v
	from = INT_INDEX(INDEX_CON_COMAND_JUMP);
	to = INT_INDEX(INDEX_ACT_COMAND_JUMP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.5f);

	//�����U���R�}���h���͏����[�������U��
	from = INT_INDEX(INDEX_CON_COMAND_FALLATTACK);
	to = INT_INDEX(INDEX_ACT_COMAND_FALLATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//�h��R�}���h���͏����[���h��
	from = INT_INDEX(INDEX_CON_COMAND_DEFENCE);
	to = INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.7f);

	//�h��[�����
	from = INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
	to = INT_INDEX(INDEX_ACT_COMAND_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.8f);

	//�K�E�Z�R�}���h���͏����[���K�E�Z
	from = INT_INDEX(INDEX_CON_COMAND_UNLEASHSKILL);
	to = INT_INDEX(INDEX_ACT_COMAND_UNLEASHSKILL);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 3.0f);

	//�J�E���^�[�U���R�}���h���͏����[���J�E���^�[�U��
	from = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
	to = INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//�J�E���^�[�U���R�}���h���͏����[���J�E���^�[�U�����s
	from = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
	to = INT_INDEX(INDEX_ACT_COMAND_FAIL_COUNTERATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);
}

bool ActorAi::ParamInitStaminaRecove()
{
	//�X�^�~�i�񕜏���
	if (nowIndex_ == NONE_INDEX ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_UP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_DOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_AUTO_RUN))
	{
		actor_.lock()->userInterface_->AddStamina(0.3f);
	}

	if (nowIndex_ == NONE_INDEX)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::IDLE)
		{
			moveFlag_ = false;
			moveDir_ = MyUtility::VECTOR_ZERO;
			comandFlag_ = false;
			avoidDir_ = MyUtility::VECTOR_ZERO;
			prevAvoidDir_ = MyUtility::VECTOR_ZERO;
			jumpFlag_ = false;
			actor_.lock()->ChangeState(ActorBase::STATE::IDLE);
			return true;
		}
	}

	return false;
}

int ActorAi::ConditionBranch()
{
	int ans = nowIndex_;

	if (mode_ == MODE::MANUAL)
	{
		//�ړ���������
		ConditionMove(ans);
		//�R�}���h���͏�������
		ConditionComand(ans);
	}

	return ans;
}

void ActorAi::ConditionMove(int& idx)
{
	if (nowIndex_ == NONE_INDEX)
	{
		//��ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_UP), idx);
		//���ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_DOWN), idx);
		//���ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_LEFT), idx);
		//�E�ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_RIGHT), idx);
		//����ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_LEFTUP), idx);
		//�E��ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_RIGHTUP), idx);
		//�����ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_LEFTDOWN), idx);
		//�E���ړ�
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN), idx);
	}
}

void ActorAi::ConditionComand(int& idx)
{
	if (nowIndex_ == NONE_INDEX ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_UP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_DOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN))
	{
		//��U��
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_WEEK), idx);
		//���U��
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_STRONG), idx);
		//���
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_AVOID), idx);
		//�����ړ�
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_QUICKMOVE), idx);
		//�W�����v
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_JUMP), idx);
		//�h��
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_DEFENCE), idx);
		//�K�E�Z
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_UNLEASHSKILL), idx);
	}
	else
	{
		//��������
		NodeTransition(nowIndex_, idx);
	}
}

Dir3DF ActorAi::FactEnemy()
{
	Dir3DF dir = VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos);
	dir = VNorm(dir);
	Quaternion q;
	Rot3DF angle = q.PosAxis(dir);
	//���[����]�͖���
	angle.y = 0.0f;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);

	return dir;
}

int ActorAi::ConditionAutoMoveFar() const
{
	//�����`�F�b�N
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis >= std::powf(DISTANCE_FAR, 2.0f))
	{
		return INT_INDEX(INDEX_ACT_AUTO_MOVE_FAR);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionAutoMoveMedium() const
{
	//�����`�F�b�N
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis >= std::powf(DISTANCE_MEDIUM, 2.0f))
	{
		return INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionAutoDefence() const
{
	return INT_INDEX(INDEX_ACT_AUTO_DEFENCE);
}

int ActorAi::ConditionAutoAvoid() const
{
	return INT_INDEX(INDEX_ACT_AUTO_AVOID);
}

int ActorAi::ConditionAutoRun() const
{
	return INT_INDEX(INDEX_ACT_AUTO_RUN);
}

int ActorAi::ConditionAutoWeek() const
{
	if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
	{
		bool random = static_cast<bool>(GetRand(100) % 100);
		if (!random)
		{
			return INT_INDEX(INDEX_ACT_COMAND_JUMP);
		}
		return INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionAutoStrong() const
{
	if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
	{
		return INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	}
	return NONE_INDEX;
}

//�ړ�����
int ActorAi::ConditionMoveUp()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_UP);
	}

	return NONE_INDEX;
}

int ActorAi::ConditionMoveDown()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT))
	{
		return INT_INDEX(INDEX_ACT_MOVE_DOWN);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveLeft()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_LEFT);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveRight()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_RIGHT);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveLeftUp()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_LEFTUP);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveRightUp()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_RIGHTUP);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveLeftDown()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveRightDown()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN);
	}

	return NONE_INDEX;
}

//�R�}���h���͏���
int ActorAi::ConditionComandWeek() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_WEEKATC))
	{
		return INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandWeekBranch()
{
	if (jumpFlag_)
	{
		return INT_INDEX(INDEX_CON_COMAND_FALLATTACK);
	}
	//�U����0��
	else if (atackWeakCnt_ <= 0)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_1);
	}
	//�U����1��
	else if (atackWeakCnt_ <= 1)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_2);
	}
	//�U����2��
	else if (atackWeakCnt_ <= 2)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_3);
	}
	//�U����3��
	else if (atackWeakCnt_ <= 3)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_4);
	}
	//�U����4��
	else if (atackWeakCnt_ <= 4)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_5);
	}
	//�U����5��
	else if (atackWeakCnt_ <= 5)
	{
		atackWeakCnt_ = 0;//�񐔃��Z�b�g
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_6);
	}

	return NONE_INDEX;
}

int ActorAi::ConditionComandStrong() const
{
	if(input.GetSimpleInput(SimpleInput::SIMPLE::KEY_STRONGATC))
	{ 
	return INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandStrongBranch()
{
	if (jumpFlag_)
	{
		return INT_INDEX(INDEX_CON_COMAND_FALLATTACK);
	}
	//�U����0��
	else if (atackStrongCnt_ <= 0)
	{
		atackStrongCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_STRONG_1);
	}
	//�U����1��
	else if (atackStrongCnt_ <= 1)
	{
		atackStrongCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_STRONG_2);
	}
	//�U����2�`5��
	else if (atackStrongCnt_ <= 2)
	{
		atackStrongCnt_ = 0;//���Z�b�g
		return INT_INDEX(INDEX_ACT_COMAND_STRONG_3);
	}

	return NONE_INDEX;
}

int ActorAi::ConditionComandAvoid() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		if (actor_.lock()->userInterface_->GetStateStamina() >= SUB_STAMINA_AVOID)
		{
			return INT_INDEX(INDEX_ACT_COMAND_AVOID);
		}
		else
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandQuickMove() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_QUICKMOVE) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		if (actor_.lock()->userInterface_->GetStateStamina() >= SUB_STAMINA_QUICKMOVE)
		{
			return INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE);
		}
		else
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandJump() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_JUMP))
	{
		if (actor_.lock()->userInterface_->GetStateStamina() >= SUB_STAMINA_JUMP)
		{
			return INT_INDEX(INDEX_ACT_COMAND_JUMP);
		}
		else
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandFallAttack() const
{
	if ((input.GetSimpleInput(SimpleInput::SIMPLE::KEY_WEEKATC) ||
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_STRONGATC)))
	{
		return INT_INDEX(INDEX_ACT_COMAND_FALLATTACK);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandDefence() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DEFENCE) &&
		timeCounter_.at(TIME::GURD_RAG) <= 0.0f)
	{
		return INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandUnleashSkill() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_SKILL_1) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_SKILL_2) &&
		actor_.lock()->IsUnleashSkill())
	{
		return INT_INDEX(INDEX_ACT_COMAND_UNLEASHSKILL);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandCounterAttack()
{
	if (timeCounter_.at(TIME::JUST_CHANCE_TIME) > 0.0f &&
		(input.GetSimpleInput(SimpleInput::SIMPLE::KEY_WEEKATC) ||
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_STRONGATC)))
	{
		//����̃t���[�����x�����ɖ߂�
		actor_.lock()->enemy_.lock()->actAi_->SetFrameDefaultSpeed();
		return INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK);
	}
	else if(timeCounter_.at(TIME::JUST_CHANCE_TIME) <= 0.0f)
	{
		//����̃t���[�����x�����ɖ߂�
		actor_.lock()->enemy_.lock()->actAi_->SetFrameDefaultSpeed();
		actor_.lock()->actorCam_->ChangeDefaultMode();
		return INT_INDEX(INDEX_ACT_COMAND_FAIL_COUNTERATTACK);
	}

	return NONE_INDEX;
}

void ActorAi::ActionAutoMoveFar()
{
	if (timeCounter_.at(TIME::QUICK_RAG) <= 0.0f &&
		nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE);
		nowCost_ = NONE_COST;
	}
	else
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MOVE)
		{
			actor_.lock()->ChangeState(ActorBase::STATE::MOVE);
		}
		else
		{
			//�G�̕��p�Ɍ���
			Dir3DF dir = FactEnemy();
			dir = VScale(dir, -1.0f);

			//�G�̕��p�Ɉړ�
			actor_.lock()->transform_->pos = VAdd(
				actor_.lock()->transform_->pos,
				VScale(dir, actor_.lock()->GetSpeed()));

			//�O������𖞂����Ȃ��Ȃ�����A�N�V�����I��
			int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_AUTO_MOVE_FAR));
			if (nSearch >= 0)
			{
				if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
				{
					EndNodeAction(nSearch);
				}
			}
		}
	}

}

void ActorAi::ActionAutoMoveMedium()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::MOVE)
	{
		actor_.lock()->ChangeState(ActorBase::STATE::MOVE);
	}
	else
	{
		//�ړ�����
		//�G�̕��p�Ɍ���
		Dir3DF dir = FactEnemy();
		dir = VScale(dir, -1.0f);

		actor_.lock()->transform_->pos = VAdd(
			actor_.lock()->transform_->pos,
			VScale(dir, actor_.lock()->GetSpeed()));

		//�O������𖞂����Ȃ��Ȃ�����A�N�V�����I��
		int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_AUTO_MOVE_MEDIUM));
		if (nSearch >= 0)
		{
			if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionAutoDefence()
{
	if (nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_DEFENCE))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
		nowCost_ = NONE_COST;
	}
}

void ActorAi::ActionAutoAvoid()
{
	if (nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_AVOID))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_AVOID);
		nowCost_ = NONE_COST;
	}
}

void ActorAi::ActionAutoRun()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::MOVE)
	{
		actor_.lock()->ChangeState(ActorBase::STATE::MOVE);
	}
	else
	{

		//�G�̕��p�Ɍ���
		Dir3DF dir = FactEnemy();
		dir = VScale(dir, -1.0f);

		actor_.lock()->transform_->pos = VAdd(
			actor_.lock()->transform_->pos,
			VScale(dir, actor_.lock()->GetSpeed()));

		//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă��疳�����ŃA�N�V�����I��
		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			EndNodeAction(nSearch);
		}
	}
}

void ActorAi::ActionAutoCounterAttack(void)
{
	if (timeCounter_.at(TIME::COUNTER_RAG) <= 0.0f && nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK);
		nowCost_ = NONE_COST;
	}
}

//�ړ��A�N�V����
void ActorAi::ActionMoveUp()
{
	//transform_->pos.z += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetForward();

	//���f���̌��̑O������-z���ł��邽��180�x��]������
	Quaternion q = Quaternion::AngleAxis(MyUtility::Deg2RadD(180.0f), MyUtility::AXIS_Y);
	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;
	
	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_UP));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionMoveDown()
{
	//transform_->pos.z -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetBack();

	Quaternion q = Quaternion();
	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_DOWN));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
				
			}
		}
	}
}

void ActorAi::ActionMoveLeft()
{
	//transform_->pos.x -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();

	Quaternion q = Quaternion();
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(90.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_LEFT));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionMoveRight()
{
	//transform_->pos.x += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();

	//���f���̌��̑O������-z���ł��邽��180�x��]������
	Quaternion q = Quaternion();

	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-90.0f), MyUtility::AXIS_Y));
	

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_RIGHT));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveLeftUp()
{
	//transform_->pos.x -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetForward());

	Quaternion q = Quaternion();

	//�����
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(135.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_LEFTUP));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveRightUp()
{
	//transform_->pos.x += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetForward());

	//���f���̌��̑O������-z���ł��邽��180�x��]������
	Quaternion q = Quaternion();

	//��������������Ă���ꍇ
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-135.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_RIGHTUP));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveLeftDown()
{
	//transform_->pos.x -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetBack());

	Quaternion q = Quaternion();
	//������
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(45.0f), MyUtility::AXIS_Y));


	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_LEFTDOWN));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveRightDown()
{
	//transform_->pos.x += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetBack());

	//���f���̌��̑O������-z���ł��邽��180�x��]������
	Quaternion q = Quaternion();

	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-45.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//���[����]�͖���
	angle.y = 0.0f;

	moveFlag_ = true;
	//�ړ�����
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//�O���������������Ȃ��Ȃ����Ƃ�
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//�ꉞ�C���f�b�N�X�̃T�[�`�����Ă���A�N�V�����I��
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

//�R�}���h�A�N�V����
void ActorAi::ActionComandWeek_1() 
{

}

void ActorAi::ActionComandWeek_2()
{

}

void ActorAi::ActionComandWeek_3()
{

}

void ActorAi::ActionComandWeek_4()
{

}

void ActorAi::ActionComandWeek_5()
{

}

void ActorAi::ActionComandWeek_6()
{

}

void ActorAi::ActionComandStrong_1() 
{

}

void ActorAi::ActionComandStrong_2()
{

}

void ActorAi::ActionComandStrong_3()
{
	
}

void ActorAi::ActionComandAvoid()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::AVOID &&
		actor_.lock()->GetState() != ActorBase::STATE::BACKSTEP)
	{
		SetTime(TIME::JUST_RECEP_TIME, 0.05f *
			static_cast<float>(actor_.lock()->userInterface_->GetStateAvoidLevel()));

		comandFlag_ = false;

		if (ConditionMoveLeft() == NONE_INDEX && ConditionMoveRight() == NONE_INDEX &&
			ConditionMoveDown() == NONE_INDEX)
		{
			//�ǂ��ɂ��ړ��L�[�������ĂȂ���Ώ�Ԃ��o�b�N�X�e�b�v�ɂ���
			actor_.lock()->ChangeState(ActorBase::STATE::BACKSTEP);
		}
		else
		{
			actor_.lock()->ChangeState(ActorBase::STATE::AVOID);
		}

		//�X�^�~�i������
		actor_.lock()->userInterface_->SubStamina(SUB_STAMINA_AVOID);
		actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
	}
	else
	{
		//�������
		//1�t���[���߂��Ă������s�����Ƃ�悤�ɂ��Ă���
		if (!comandFlag_)
		{
			comandFlag_ = true;
			float power = actor_.lock()->GetAvoidPower();

			if (ConditionMoveLeft() != NONE_INDEX)
			{
				Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();
				
				Quaternion q = Quaternion();

				//�����
				q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(135.0f), MyUtility::AXIS_Y));

				Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
				//���[����]�͖���
				angle.y = 0.0f;

				//�ړ�����
				actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
				//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
				avoidDir_ = dir;
				power *= 0.8f;
				if (power < AVOID_POWER_MIN)power = AVOID_POWER_MIN;
				else if (power > AVOID_POWER_MAX)power = AVOID_POWER_MAX;
			}
			else if (ConditionMoveRight() != NONE_INDEX)
			{
				Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();

				Quaternion q = Quaternion();

				//�����
				q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-135.0f), MyUtility::AXIS_Y));

				Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
				//���[����]�͖���
				angle.y = 0.0f;

				//�ړ�����
				actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
				//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
				avoidDir_ = dir;
				power *= 0.8f;
			}
			else if (ConditionMoveDown() != NONE_INDEX)
			{
				Dir3DF dir = actor_.lock()->camera_.lock()->GetBack();

				Quaternion q = Quaternion();

				//�����
				q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(67.0f), MyUtility::AXIS_Y));

				Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
				//���[����]�͖���
				angle.y = 0.0f;

				//�ړ�����
				actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
				//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
				avoidDir_ = dir;
				power *= 0.8f;
				if (power < AVOID_POWER_MIN)power = AVOID_POWER_MIN;
				else if (power > AVOID_POWER_MAX)power = AVOID_POWER_MAX;
			}
			else
			{
				//�ǂ��ɂ��ړ����Ă��Ȃ��Ƃ��̓v���C���[�̌�둤�ɃZ�b�g�����
				avoidDir_ = actor_.lock()->actorCam_->GetBack();

				power *= 0.6f;
				if (power < AVOID_POWER_MIN)power = AVOID_POWER_MIN;
				else if (power > AVOID_POWER_MAX)power = AVOID_POWER_MAX;
				//�o�b�N�X�e�b�v�̓W�����v�x�N�g���ŕ\��
				actor_.lock()->SetJumpVector({ 0.0f, power, 0.0f });
			}

			avoidDir_ = VScale(avoidDir_, power);
			prevAvoidDir_ = avoidDir_;
		}
		else if(actor_.lock()->animController_->GetStepAnimTime() > 0.05f)
		{
			//����x�N�g���̕����Ɖ�������O����x�N�g���̕����������ɂȂ��������I��
			if (VDot(VNorm(avoidDir_), VNorm(prevAvoidDir_)) < 0 &&
				actor_.lock()->transform_->pos.y <= 0)
			{
				comandFlag_ = false;
				avoidDir_ = MyUtility::VECTOR_ZERO;
				prevAvoidDir_ = avoidDir_;

				actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::AVOID);
				//�����ő���̕�������
				FactEnemy();

				//�A�N�V�������I��
				int nSearch = SearchNodeIdx(nowIndex_);
				if (nSearch >= 0)
				{
					EndNodeAction(nSearch);
				}
			}

			//�������
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, avoidDir_);

			//����p���[�͏��X�ɉ�����
			//�������̋t���Z�o
			VECTOR subVec = VScale(VNorm(avoidDir_), -1.0f);
			subVec = VScale(subVec, 3.0f);

			avoidDir_ = VAdd(avoidDir_, subVec);
		}


		//�W���X�g����
		if (timeCounter_.at(TIME::JUST_RECEP_TIME) > 0.0f && damageFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			damageFlag_ = false;
			SetTime(TIME::JUST_CHANCE_TIME, 1.5f);
			actor_.lock()->actorCam_->ChangeMode(Camera::MODE::JUST);
			actor_.lock()->enemy_.lock()->SetStopFrame(1.5f);
			actor_.lock()->userInterface_->SetDamagePower(0.0f);
			actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::JUST);
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);
			actor_.lock()->SetSwayVector(MyUtility::VECTOR_ZERO);

			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::JUST, ActorBase::USE_EFFECT::JUST, effPos, effRot);

			actor_.lock()->SoundPlay(SoundManager::SOD::JUST, DX_PLAYTYPE_BACK);

			//�A�N�V�����̏I������
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
			//�J�E���^�[�A�^�b�N�����ɑJ��
			if (mode_ == MODE::MANUAL)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
			}
			else
			{
				nowIndex_ = INT_INDEX(INDEX_ACT_AUTO_COUNTER);
				SetTime(TIME::COUNTER_RAG, 0.3f);
			}
			nowCost_ = NONE_COST;
		}
	}
}

void ActorAi::ActionComandQuickMove()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE)
	{
		//��Ԃ������ړ��ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::QUICK_MOVE);
		SetTime(TIME::QUICK_TIME, 0.65f);
		//�����蔻��t���O
		colliderFlag_ = false;

		actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::QUICK_MOVE);
		//�G�t�F�N�g
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(-1.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		actor_.lock()->PlayEffect(EffectManager::EFF::QUICK_MOVE, ActorBase::USE_EFFECT::QUICK_MOVE, effPos, effRot);
		//�X�^�~�i������
		actor_.lock()->userInterface_->SubStamina(SUB_STAMINA_QUICKMOVE);
		actor_.lock()->SoundPlay(SoundManager::SOD::QUICK_MOVE, DX_PLAYTYPE_BACK);
	}
	else
	{
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(-1.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		//�G�t�F�N�g�̍��W�����킹��
		actor_.lock()->SetEffectPos(EffectManager::EFF::QUICK_MOVE, ActorBase::USE_EFFECT::QUICK_MOVE, effPos);
		//�G�t�F�N�g�̉�]�����킹��
		actor_.lock()->SetEffectRot(EffectManager::EFF::QUICK_MOVE, ActorBase::USE_EFFECT::QUICK_MOVE, effRot);

		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (!colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < 100; ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->prevPos, dir));

				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;

					if (actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_QUICKMOVE)
					{
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					}

					//����̃A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						if (nSearch >= 0)
						{
							actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						}
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					//�����̃A�N�V�������I��
					nSearch = SearchNodeIdx(nowIndex_);
					if (nSearch >= 0)
					{
						EndNodeAction(nSearch);
					}
					break;
				}
			}
		}


		//�����ړ�����
		//1�t���[���߂��Ă��獂���ړ�����悤�ɂ��Ă���
		//�A�j���[�V������5�X�e�b�v�i�񂾂��ƂɈړ�
		if (actor_.lock()->animController_->GetStepAnimTime() > 5.0f)
		{
			//�����ő���̕�������
			Dir3DF dir = FactEnemy();
			dir = VScale(dir, -1.0f);

			VECTOR quickMoveVec_ = VScale(dir, ActorBase::QUICK_SPEED);
			//�����ړ�����
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, quickMoveVec_);
		}
		else if (timeCounter_.at(TIME::QUICK_TIME) <= 0.0f)
		{
			//�����ړ��^�C����0�ɂȂ�����A��������
			//�G�Ɏ����̑̂����������珈���I��
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}


void ActorAi::ActionComandJump() 
{
	if (actor_.lock()->GetState() != ActorBase::STATE::JUMP)
	{
		//��Ԃ��W�����v�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::JUMP);
		//�X�^�~�i������
		actor_.lock()->userInterface_->SubStamina(SUB_STAMINA_JUMP);
		actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
	}
	else
	{
		//�ړ����Ȃ炻�̕����ɃW�����v�����ړ�����
		if (moveFlag_)
		{
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(moveDir_, actor_.lock()->GetSpeed()));
		}

		//�W�����v����
		//1�t���[���߂��Ă���W�����v����悤�ɂ��Ă���
		if (!jumpFlag_)
		{
			//�W�����v�Ƃ������㉺�����ɓ����x�N�g��
			actor_.lock()->SetJumpVector(VGet(0.0f, actor_.lock()->GetJumpPower(), 0.0f));
			jumpFlag_ = true;

			//�G�t�F�N�g�̔���
			Position3DF effPos = actor_.lock()->transform_->pos;
			Rot3DF effRot = MyUtility::VECTOR_ZERO;
			actor_.lock()->PlayEffect(EffectManager::EFF::JUMP, ActorBase::USE_EFFECT::JUMP, effPos, effRot);
		}
		//�����U���̏������W�����v���ɖ��������ꍇ
		else if ((nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_FALLATTACK))]->IsCondition() != NONE_INDEX) ||
				(mode_ == MODE::AUTO && actor_.lock()->animController_->GetStepAnimTime() >= 40.0f))
		{
			nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_FALLATTACK);
			nowCost_ = NONE_COST;
		}
		//�n�ʂȂǂɓ��������ꍇ
		else if (actor_.lock()->IsCollGroundFlag())
		{
			//�W�����v�̃L�����Z��
			//�W�����v�Ƃ������㉺�����ɓ����x�N�g��
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);
			jumpFlag_ = false;

			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionComandFallAttack() 
{
	if (actor_.lock()->GetState() != ActorBase::STATE::FALL_ATTACK)
	{
		//��Ԃ𗎉��U���ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::FALL_ATTACK);
		SetTime(TIME::ATTACK_RAG, 0.8f);
		//�X�^�~�i�����炷
		actor_.lock()->userInterface_->SubStamina(10.0f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 35.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			SphereF sph = actor_.lock()->Model2Sphere(250);
			if (actor_.lock()->collider_->CollisionSphere2Cap(sph))
			{
				//�����蔻��t���O
				colliderFlag_ = true;
				float power = actor_.lock()->GetAttackPower();
				actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
				actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() / 1.3f ,0.0f });
				actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 100.0f));
				actor_.lock()->camera_.lock()->SetPos(
					VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetBack(), 100.0f)));
				actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);

				actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);

				SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
			}
		}


		//�����U������
		if (!comandFlag_)
		{
			comandFlag_ = true;
			actor_.lock()->SetJumpVector(VGet(0.0f, actor_.lock()->GetJumpPower() / 2.0f, 0.0f));
		}
		//�n�ʂȂǂɓ��������ꍇ
		else if (actor_.lock()->IsCollGroundFlag())
		{
			comandFlag_ = false;
			//�W�����v�̃L�����Z��
			//�W�����v�Ƃ������㉺�����ɓ����x�N�g��
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);;
			//�W�����v�̔h���ŗ����U�����s������
			//�W�����v�t���O���I�t�ɂ���
			jumpFlag_ = false;

			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}

			//�n�ʂɒ�������ɁA�����蔻��p�̃G�t�F�N�g�𔭐�������
			if (colliderFlag_)
			{
				Position3DF effPos = VAdd(actor_.lock()->transform_->pos, VGet(0.0f, 50.0f, 0.0f));
				Rot3DF effRot = MyUtility::VECTOR_ZERO;
				actor_.lock()->PlayEffect(EffectManager::EFF::FALL_ATTACK, ActorBase::USE_EFFECT::FALL_ATTACK, effPos, effRot);
			}

			//���I�E���G�t�F�N�g�̔���
			Position3DF effPos = VAdd(actor_.lock()->transform_->pos, VGet(0.0f, 50.0f, 0.0f));
			Rot3DF effRot = MyUtility::VECTOR_ZERO;
			effRot = Quaternion::ToEuler(
				Quaternion::AngleAxis(MyUtility::Deg2RadD(-50.0), MyUtility::AXIS_X));
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
		}
	}
}

void ActorAi::ActionComandDefence() 
{
	if (actor_.lock()->GetState() != ActorBase::STATE::DEFENCE)
	{
		//��Ԃ�h��ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::DEFENCE);
		SetTime(TIME::JUST_RECEP_TIME, 0.02f *
			static_cast<float>(actor_.lock()->userInterface_->GetStateAvoidLevel()));

		//�G�t�F�N�g
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		actor_.lock()->PlayEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA, effPos, effRot);
		//�h���
		actor_.lock()->userInterface_->AddDefence(actor_.lock()->GetStateDefenceMax());
		
		//�����ő���̕��Ɍ���
		FactEnemy();
	}
	else
	{
		//�G�t�F�N�g
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		actor_.lock()->SetEffectPos(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA, effPos);
		actor_.lock()->SetEffectRot(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA, effRot);
		actor_.lock()->SetWeight(1.7f);
		comandFlag_ = true;

		//�h�䂵��������薂�͉�
		actor_.lock()->userInterface_->AddMagicPower(0.4f);
		//�h�䂵���������X�^�~�i����
		actor_.lock()->userInterface_->SubStamina(0.3f);

		//�K�[�h�u���C�N
		if (actor_.lock()->userInterface_->GetStateStamina() <= 0.0f &&
			!actor_.lock()->IsDamageAnimFlag())
		{
			//�ĂуK�[�h�ł���܂ł̃��O
			SetTime(TIME::GURD_RAG, 1.0f);

			actor_.lock()->SetDefaultWeight();
			actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(50.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::GURD_BREAK, ActorBase::USE_EFFECT::GURD_BREAK, effPos, effRot);

			actor_.lock()->userInterface_->SetDefence(0.0f);
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
			actor_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageGurdBreak(); });
			actor_.lock()->SoundPlay(SoundManager::SOD::GURD_BREAK, DX_PLAYTYPE_BACK);
		}

		if (mode_ == MODE::MANUAL)
		{
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_DEFENCE))]->IsCondition() == NONE_INDEX)
			{
				actor_.lock()->SetDefaultWeight();
				actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
				actor_.lock()->userInterface_->SetDefence(0.0f);
				comandFlag_ = false;
				//�A�N�V�������I��
				int nSearch = SearchNodeIdx(nowIndex_);
				if (nSearch >= 0)
				{
					EndNodeAction(nSearch);
				}
			}
			else if (ConditionComandAvoid() != NONE_INDEX)
			{
				actor_.lock()->SetDefaultWeight();
				actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
				actor_.lock()->userInterface_->SetDefence(0.0f);
				comandFlag_ = false;
				//�A�N�V������J��
				nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_AVOID);
				nowCost_ = NONE_COST;
			}
		}
		else
		{
			if (actor_.lock()->animController_->GetStepAnimTime() >= 50.0f)
			{
				actor_.lock()->SetDefaultWeight();
				actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
				actor_.lock()->userInterface_->SetDefence(0.0f);
				comandFlag_ = false;
				//�A�N�V�������I��
				int nSearch = SearchNodeIdx(nowIndex_);
				if (nSearch >= 0)
				{
					EndNodeAction(nSearch);
				}
			}
		}

		//�W���X�g����
		if (timeCounter_.at(TIME::JUST_RECEP_TIME) > 0.0f && damageFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			damageFlag_ = false;
			SetTime(TIME::JUST_CHANCE_TIME, 1.5f);
			actor_.lock()->actorCam_->ChangeMode(Camera::MODE::JUST);
			actor_.lock()->enemy_.lock()->SetStopFrame(1.5f);
			actor_.lock()->userInterface_->SetDamagePower(0.0f);
			actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::JUST);
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);
			actor_.lock()->SetSwayVector(MyUtility::VECTOR_ZERO);

			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::JUST, ActorBase::USE_EFFECT::JUST, effPos, effRot);

			actor_.lock()->SoundPlay(SoundManager::SOD::JUST, DX_PLAYTYPE_BACK);

			//�A�N�V�����̏I������
			actor_.lock()->SetDefaultWeight();
			actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
			actor_.lock()->userInterface_->SetDefence(0.0f);

			//���͉�
			actor_.lock()->userInterface_->AddMagicPower(20.0f);

			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
			//�J�E���^�[�A�^�b�N�����ɑJ��
			if (mode_ == MODE::MANUAL)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
			}
			else
			{
				nowIndex_ = INT_INDEX(INDEX_ACT_AUTO_COUNTER);
				SetTime(TIME::COUNTER_RAG, 0.3f);
			}
			nowCost_ = NONE_COST;
		}
		//�h���ԂŃ_���[�W���󂯂��Ƃ��A�h��͌o���l�A�b�v
		if (damageFlag_)
		{
			damageFlag_ = false;
			actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);
			//�X�^�~�i������
			actor_.lock()->userInterface_->SubStamina(5.0f);
		}
	}
}

void ActorAi::ActionComandUnleashSkill() 
{
	
}

void ActorAi::ActionComandCounterAttack() 
{
	//��ɑ���̕�������
	//�����ő���̕�������
	FactEnemy();

	if (actor_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK)
	{
		//��Ԃ���U��_1�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::COUNTER_ATTACK);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 0.5f);

		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 10.0f && !colliderFlag_)
		{
			VERTEX3D vertex[3];

			for (int ii = 1; ii < COUNTER_ATTACK_AREA; ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() / 2.3f ,0.0f });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 150.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetUp(), 180.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
					}
					//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
					actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();

					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
					Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
					actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG, effPos, effRot);

					effPos = actor_.lock()->GetPlayerForwardPos(50.0f);
					actor_.lock()->PlayEffect(EffectManager::EFF::BLUR, ActorBase::USE_EFFECT::BLUR, effPos, effRot);

					//�G�t�F�N�g
					actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG_2, effPos, effRot);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}


		if (!comandFlag_)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			Position3DF lockonPos = actor_.lock()->enemy_.lock()->transform_->pos;
			Dir3DF dir = VSub(actor_.lock()->transform_->pos,
				actor_.lock()->enemy_.lock()->transform_->pos);
			dir = VNorm(dir);
			lockonPos = VAdd(lockonPos, VScale(dir, 150.0f));
			actor_.lock()->transform_->pos = lockonPos;

			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::JUST, ActorBase::USE_EFFECT::JUST, effPos, effRot);
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			actor_.lock()->actorCam_->ChangeDefaultMode();
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionComandFailCounterAttack()
{
	//�A�N�V�������I��
	int nSearch = SearchNodeIdx(nowIndex_);
	if (nSearch >= 0)
	{
		EndNodeAction(nSearch);
	}
}