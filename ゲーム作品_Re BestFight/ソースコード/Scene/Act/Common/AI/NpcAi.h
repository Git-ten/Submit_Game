#pragma once
#include "BehaviorBase.h"

class ActorBase;
class ActorAi;

class NpcAi : public BehaviorBase
{
public:

	//�R���X�g���N�^
	NpcAi(std::weak_ptr<ActorAi>targetAi, std::weak_ptr<ActorBase>actor);

	//�f�X�g���N�^
	~NpcAi(void)override;

	//�X�V
	void Update(void)override;

	void DebugDraw(void)override;

private:

	//�C���f�b�N�X��
	enum INDEX
	{
		//���I������
		INDEX_CON_OPERATION_MOVE = 1,
		INDEX_CON_OPERATION_ESCAPE,
		INDEX_CON_OPERATION_ATTACK,

		//�ړ�����
		INDEX_CON_MOVE,
		INDEX_CON_MOVE_BRANCH,

		//�ړ�����
		INDEX_ACT_MOVE_FAR,
		INDEX_ACT_MOVE_MEDIUM,

		//�������
		INDEX_CON_ESCAPE,
		INDEX_CON_ESCAPE_BRANCH,

		//�������
		INDEX_ACT_ESCAPE_AVOID,
		INDEX_ACT_ESCAPE_RUN,
		INDEX_ACT_ESCAPE_DEFENCE,

		//�U������
		INDEX_CON_ATTACK,
		INDEX_CON_ATTACK_BRANCH,

		//�U������
		INDEX_ACT_ATTACK_WEEK,
		INDEX_ACT_ATTACK_STRONG,
		
		//�K�E�Z����
		INDEX_ACT_UNLEASHSKILL,
	};

	//�g�p����AI
	std::weak_ptr<ActorAi>targetAi_;
	//�ΏۃA�N�^�[
	std::weak_ptr<ActorBase>actor_;



	//�����ł̂ݎg�p����֐�
	void CreateNode(void)override;
	void ConnectNode(void)override;
	int ConditionBranch(void)override;
	void ConditionStartNode(int& idx);

	//�m�[�h�ɓo�^����p�̊֐�
	//���I������
	int ConditionOperateMove(void);
	int ConditionOperateEscape(void);
	int ConditionOperateAttack(void);

	//�ړ�����
	int ConditionMove(void);
	int ConditionMoveBranch(void);

	//�������
	int ConditionEscape(void);
	int ConditionEscapeBranch(void);

	//�U������
	int ConditionAttack(void);
	int ConditionAttackBranch(void);

	//�A�N�V�����̎���
	//�ړ��A�N�V����
	void ActionMoveFar(void);
	void ActionMoveMedium(void);
	
	//����A�N�V����
	void ActionEscapeAvoid(void);
	void ActionEscapeRun(void);
	void ActionEscapeDefence(void);

	//�U���A�N�V����
	void ActionAttackWeek(void);
	void ActionAttackStrong(void);

	//�K�E�Z�A�N�V����
	void ActionUnleashSkill(void);
};