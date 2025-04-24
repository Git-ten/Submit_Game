#pragma once
#include<memory>
#include<map>
#include "BehaviorBase.h"
#include"../../../../Utility/VectorHeader.h"

class ActorBase;

class ActorAi : public BehaviorBase
{
public:

	//AI���[�h
	enum class MODE
	{
		AUTO,//CPU����
		MANUAL,//�蓮
	};

	//����
	enum class TIME
	{
		FRAME_TIME,//�t���[���^�C��
		ATTACK_RAG,//�U���̃��O
		QUICK_TIME,//�����ړ�����
		QUICK_RAG,//�����ړ��̃��O
		AVOID_TIME,//�������
		JUST_RECEP_TIME,//�W���X�g���莞��
		JUST_CHANCE_TIME,//�W���X�g�J�E���^�[�`�����X����
		GURD_RAG,//�K�[�h�u���C�N���ꂽ��ĂуK�[�h�ł���܂ł̎���

		//�������[�h��p����
		COUNTER_RAG,//�J�E���^�[�U��������܂ł̃��O
	};

	//�C���f�b�N�X��
	enum INDEX
	{
		//�ړ�����
		//����
		INDEX_CON_AUTO_MOVE_FAR = 1,
		INDEX_CON_AUTO_MOVE_MEDIUM,

		//���������
		//����
		INDEX_CON_AUTO_DEFENCE,
		INDEX_CON_AUTO_AVOID,
		INDEX_CON_AUTO_RUN,

		//�U������
		//����
		INDEX_CON_AUTO_WEEK,
		INDEX_CON_AUTO_STRONG,

		//�ړ��A�N�V����
		//����
		INDEX_ACT_AUTO_MOVE_FAR,
		INDEX_ACT_AUTO_MOVE_MEDIUM,

		//������A�N�V����
		//����
		INDEX_ACT_AUTO_DEFENCE,
		INDEX_ACT_AUTO_AVOID,
		INDEX_ACT_AUTO_RUN,

		//�J�E���^�[�U���A�N�V����
		//����
		INDEX_ACT_AUTO_COUNTER,

		//�ړ�����
		//�蓮
		INDEX_CON_MOVE_UP,
		INDEX_CON_MOVE_DOWN,
		INDEX_CON_MOVE_LEFT,
		INDEX_CON_MOVE_RIGHT,
		INDEX_CON_MOVE_LEFTUP,
		INDEX_CON_MOVE_RIGHTUP,
		INDEX_CON_MOVE_LEFTDOWN,
		INDEX_CON_MOVE_RIGHTDOWN,

		//�蓮
		INDEX_ACT_MOVE_UP,
		INDEX_ACT_MOVE_DOWN,
		INDEX_ACT_MOVE_LEFT,
		INDEX_ACT_MOVE_RIGHT,
		INDEX_ACT_MOVE_LEFTUP,
		INDEX_ACT_MOVE_RIGHTUP,
		INDEX_ACT_MOVE_LEFTDOWN,
		INDEX_ACT_MOVE_RIGHTDOWN,

		//�R�}���h���͏���
		INDEX_CON_COMAND_WEEK,
		INDEX_CON_COMAND_WEEKBRANCH,
		INDEX_CON_COMAND_STRONG,
		INDEX_CON_COMAND_STRONGBRANCH,
		INDEX_CON_COMAND_AVOID,
		INDEX_CON_COMAND_QUICKMOVE,
		INDEX_CON_COMAND_JUMP,
		INDEX_CON_COMAND_FALLATTACK,
		INDEX_CON_COMAND_DEFENCE,
		INDEX_CON_COMAND_UNLEASHSKILL,
		INDEX_CON_COMAND_COUNTERATTACK,

		//�R�}���h�A�N�V����
		INDEX_ACT_COMAND_WEEK_1,
		INDEX_ACT_COMAND_WEEK_2,
		INDEX_ACT_COMAND_WEEK_3,
		INDEX_ACT_COMAND_WEEK_4,
		INDEX_ACT_COMAND_WEEK_5,
		INDEX_ACT_COMAND_WEEK_6,
		INDEX_ACT_COMAND_STRONG_1,
		INDEX_ACT_COMAND_STRONG_2,
		INDEX_ACT_COMAND_STRONG_3,
		INDEX_ACT_COMAND_AVOID,
		INDEX_ACT_COMAND_QUICKMOVE,
		INDEX_ACT_COMAND_JUMP,
		INDEX_ACT_COMAND_FALLATTACK,
		INDEX_ACT_COMAND_DEFENCE,
		INDEX_ACT_COMAND_UNLEASHSKILL,
		INDEX_ACT_COMAND_COUNTERATTACK,
		INDEX_ACT_COMAND_FAIL_COUNTERATTACK,
	};

	//����Ƃ̋����
	//������
	static constexpr float DISTANCE_FAR = 1500.0f;
	//������
	static constexpr float DISTANCE_MEDIUM = 100.0f;

	//�f�t�H���g�̃t���[������
	static constexpr float DEFAULT_FRAME_TIME = 0.01f;
	//�f�t�H���g�̃t���[�����x
	static constexpr float DEFAULT_FRAME_SPEED = 1.0f;

	//�R���X�g���N�^
	ActorAi(std::weak_ptr<ActorBase> actor);

	//�f�X�g���N�^
	virtual ~ActorAi(void)override;

	//�X�V
	void Update(void)override;

	//���Ԍn
	void SetTime(const TIME& type, const float t);
	void SetFrameSpeed(float speed);
	void SetFrameDefaultSpeed(void);

	//�_���[�W�t���O���擾
	bool GetDamageFlag(void)const;
	//�_���[�W�t���O���Z�b�g
	void SetDamageFlag(bool flg);

	//�U���񐔂����Z�b�g
	void ResetAttackCount(void);

	//���[�h�`�F���W
	void ChangeMode(const MODE& mode);
	MODE GetMode(void)const;

	//�����蔻��t���O
	bool CollFlag(void);

protected:

	//�U���q�b�g���̉�ʗh���
	static constexpr int ATTACK_VIBRATION_CNT = 15;
	//�J�E���^�[�U���̍U���͈�
	static constexpr int COUNTER_ATTACK_AREA = 400;
	//������̃X�^�~�i�����
	static constexpr float SUB_STAMINA_AVOID = 30.0f;
	//�W�����v���̃X�^�~�i�����
	static constexpr float SUB_STAMINA_JUMP = 10.0f;
	//�����ړ����̃X�^�~�i�����
	static constexpr float SUB_STAMINA_QUICKMOVE = 50.0f;
	//���͂̍ő�l
	static constexpr float AVOID_POWER_MAX = 100.0f;
	//���͂̍ŏ��l
	static constexpr float AVOID_POWER_MIN = 40.0f;

	using UpdateFunc = std::function<void(void)>;

	//�t���[�����x
	float frameSpeed_;

	MODE mode_;
	UpdateFunc updateFunc_;

	//�f�o�b�O�p�̎���
	std::map<TIME, float>timeCounter_;

	//AI�ɂ�葀��Ώ�
	std::weak_ptr<ActorBase>actor_;

	//�ړ����Ă��邩
	bool moveFlag_;
	//�ړ�����
	Dir3DF moveDir_;
	//�R�}���h�F��U���̉񐔂��L�^
	int atackWeakCnt_;
	//�R�}���h�F���U���̉񐔂��L�^
	int atackStrongCnt_;
	//�R�}���h�������Ă��邩
	bool comandFlag_;
	//������
	Dir3DF avoidDir_;
	//�O��̉�����
	Dir3DF prevAvoidDir_;
	//�W�����v���Ă��邩
	bool jumpFlag_;
	
	//�����蔻��t���O
	bool colliderFlag_;
	//�W���X�g����Ɏg��
	bool damageFlag_;



	//�����ł̂ݎg�p����֐�
	void InitTime(void);
	void UpdateTime(void);
	void UpdateAuto(void);
	void UpdateManual(void);
	void CreateNode(void)override;
	void ConnectNode(void)override;
	//�����t���ŏ���������
	bool ParamInitStaminaRecove(void);
	//��������
	int ConditionBranch(void)override;
	//�ړ�����
	void ConditionMove(int& idx);
	//�R�}���h���͏���
	void ConditionComand(int& idx);
	//�ΏۃA�N�^�[���G�A�N�^�[�̂�������Ɍ���
	Dir3DF FactEnemy(void);

	//�m�[�h�ɓo�^����p�̊֐�
	//�����F������
	int ConditionAutoMoveFar(void)const;
	//�����F������
	int ConditionAutoMoveMedium(void)const;
	//�����F�K�[�h
	int ConditionAutoDefence(void)const;
	//�����F���
	int ConditionAutoAvoid(void)const;
	//�����F��������
	int ConditionAutoRun(void)const;
	//�����F��U��
	int ConditionAutoWeek(void)const;
	//�����F��E���U��
	int ConditionAutoStrong(void)const;

	//��
	int ConditionMoveUp(void)const;
	//��
	int ConditionMoveDown(void)const;
	//��
	int ConditionMoveLeft(void)const;
	//�E
	int ConditionMoveRight(void)const;
	//����
	int ConditionMoveLeftUp(void)const;
	//�E��
	int ConditionMoveRightUp(void)const;
	//����
	int ConditionMoveLeftDown(void)const;
	//�E��
	int ConditionMoveRightDown(void)const;

	//��U��
	int ConditionComandWeek(void)const;
	//��U������
	int ConditionComandWeekBranch(void);
	//���U��
	int ConditionComandStrong(void)const;
	//���U������
	int ConditionComandStrongBranch(void);
	//����s��
	int ConditionComandAvoid(void)const;
	//�����ړ�
	int ConditionComandQuickMove(void)const;
	//�W�����v
	int ConditionComandJump(void)const;
	//�����U��
	int ConditionComandFallAttack(void)const;
	//�h��
	int ConditionComandDefence(void)const;
	//�K�E�Z
	int ConditionComandUnleashSkill(void)const;
	//�J�E���^�[�A�^�b�N
	int ConditionComandCounterAttack(void);

	//�A�N�V�����̎���
	//�����F������
	void ActionAutoMoveFar(void);
	//�����F������
	void ActionAutoMoveMedium(void);
	//�����F�K�[�h
	void ActionAutoDefence(void);
	//�����F���
	void ActionAutoAvoid(void);
	//�����F��������
	void ActionAutoRun(void);
	//�����F�J�E���^�[�A�^�b�N
	void ActionAutoCounterAttack(void);

	//�ړ��A�N�V����
	//��
	void ActionMoveUp(void);
	//��
	void ActionMoveDown(void);
	//��
	void ActionMoveLeft(void);
	//�E
	void ActionMoveRight(void);
	//����
	void ActionMoveLeftUp(void);
	//�E��
	void ActionMoveRightUp(void);
	//����
	void ActionMoveLeftDown(void);
	//�E��
	void ActionMoveRightDown(void);

	//�R�}���h�A�N�V����
	//��U��1
	virtual void ActionComandWeek_1(void);
	//��U��2
	virtual void ActionComandWeek_2(void);
	//��U��3
	virtual void ActionComandWeek_3(void);
	//��U��4
	virtual void ActionComandWeek_4(void);
	//��U��5
	virtual void ActionComandWeek_5(void);
	//��U��6
	virtual void ActionComandWeek_6(void);
	//���U��1
	virtual void ActionComandStrong_1(void);
	//���U��2
	virtual void ActionComandStrong_2(void);
	//���U��3
	virtual void ActionComandStrong_3(void);
	//����s��
	void ActionComandAvoid(void);
	//�����ړ�
	void ActionComandQuickMove(void);
	//�W�����v
	void ActionComandJump(void);
	//�����U��
	void ActionComandFallAttack(void);
	//�h��
	void ActionComandDefence(void);
	//�K�E�Z
	virtual void ActionComandUnleashSkill(void);
	//�J�E���^�[�A�^�b�N
	void ActionComandCounterAttack(void);
	//�J�E���^�[�A�^�b�N���s
	void ActionComandFailCounterAttack(void);

};