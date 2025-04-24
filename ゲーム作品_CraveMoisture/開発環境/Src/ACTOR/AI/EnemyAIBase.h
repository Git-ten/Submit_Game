#pragma once
#include <map>
#include <memory>
#include "BehaviorBase.h"
#include "../Enemy/EnemyState.h"
#include "../Enemy/EnemyData.h"
#include "../../Common/Time.h"
#include "../../Common/IsDistance.h"

class ShareData;

class EnemyAIBase : public BehaviorBase
{
protected:

	using DIR_BACK = std::function<Dir3DF(void)>;

public:

	//���O�^�C�}�[�̖��O
	enum class RAG_NAME
	{
		ATTACK_1_RAG_TIME,		// �U��1���O�^�C��
		ATTACK_2_RAG_TIME,		// �U��2���O�^�C��
		ATTACK_3_RAG_TIME,		// �U��3���O�^�C��
		ATTACK_4_RAG_TIME,		// �U��4���O�^�C��
		ATTACK_5_RAG_TIME,		// �U��5���O�^�C��
		ATTACK_6_RAG_TIME,		// �U��6���O�^�C��
		ATTACK_AWAKE_RAG_TIME,	// �o���U�����O�^�C��
	};

	//�R���X�g���N�^
	EnemyAIBase(EnemyState::STATE& state,
		const DIR_BACK& dirBack,
		const EnemyData::RagTimesData& ragTimes);

	//�f�X�g���N�^
	virtual ~EnemyAIBase(void)override;

	// ������������N���X�̃Z�b�g�A�b�v
	// ��������Ȃ��Ƌ�������𐳏�ɂł��Ȃ�
	void SetUpIsDistance(Position3DF& enemyPos, Position3DF& playerPos);
	// ��������N���X��nullptr�`�F�b�N
	const bool IsDistanceNull(void);

	// �w�肵�����O�^�C�����Z�b�g
	void SetUpRagTime(const RAG_NAME& ragTimeName);

	// �U������
	const bool IsAttack(void)const;

protected:

	// �m�[�h�̖��O
	enum class NODE_NAME
	{
		PARENT_NODE,				// �e�m�[�h
		ACTION_WOLK_NODE,			// �ړ��A�N�V�����m�[�h
		ACTION_ATTACK_1_NODE,		// �U��1�A�N�V�����m�[�h
		ACTION_ATTACK_2_NODE,		// �U��2�A�N�V�����m�[�h
		ACTION_ATTACK_3_NODE,		// �U��3�A�N�V�����m�[�h
		ACTION_ATTACK_4_NODE,		// �U��4�A�N�V�����m�[�h
		ACTION_ATTACK_5_NODE,		// �U��5�A�N�V�����m�[�h
		ACTION_ATTACK_6_NODE,		// �U��6�A�N�V�����m�[�h
		ACTION_ATTACK_AWAKE_NODE,	// �o���U���A�N�V�����m�[�h
		ACTION_END_SHORT_NODE,		// �U����Z�����Ԓ�~����m�[�h
		ACTION_END_LONG_NODE,		// �U���𒷂����Ԓ�~����m�[�h
	};


protected:

	using CONDITION = std::function<bool(void)>;
	using NODE_STRINGS = std::map<std::string, NODE_NAME>;
	using CONDITION_STRINGS = std::map<std::string, CONDITION>;

	// ���O�^�C��
	struct RagTimes
	{
	private:

		using RAG_TIME = std::map<RAG_NAME, Time>;
		using RAG_TIME_MAX = std::map<RAG_NAME, float>;

	public:

		// �U���Ȃǂ̃��O���v��
		RAG_TIME time;
		// ���O�̍ő�l���Z�b�g
		RAG_TIME_MAX time_Max;

	};

	// �m�[�h�̖��O�Ɨ񋓌^�Ƃ̑Ή��t��
	NODE_STRINGS node_NameStrings_;
	// �J�ڏ����̖��O�Ɗ֐��|�C���^�Ƃ̑Ή��t��
	CONDITION_STRINGS condition_NameStrings_;

private:

	using NAMES = std::vector<std::string>;

protected:
	
	// ���݂̏��
	EnemyState::STATE& nowState_;

	// �����n���̏���������s��
	std::unique_ptr<IsDistance> distance_;

	// ���O�^�C��
	RagTimes ragTimes_;

	// ���f���̌��������擾
	const DIR_BACK dirBack_;

	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	// ���O�̏�����
	virtual void InitRagTime(const EnemyData::RagTimesData& ragTimes);
	// ���O�^�C���̌v��
	void UpdateRagTime(void);


	//�m�[�h�쐬
	virtual void CreateNode(void)override;
	//�m�[�h���G�b�W�Ō���
	virtual void ConnectEdge(void)override;

	// �m�[�h�̖��O�Ɨ񋓌^��Ή��t����
	void SetUpNodeNameStrings(void);
	// ���O�w��ŗ񋓌^��Ԃ�
	const NODE_NAME& NodeNameStringToNODE_NAME(const std::string& name);
	const std::vector<int> NodeNameStringToNODE_NAMEVec(const NAMES& names);

	// �J�ڏ����̖��O�ƑJ�ڏ����֐���Ή��t����
	virtual void SetUpConditionNameString(void);

	// �J�ڏ����֐����擾
	const CONDITION& ConditionNameStringToCondition(const std::string& name);
	const std::vector<CONDITION>ConditionNameStringToConditionVec(const NAMES& names);


	// ��ԑJ��
	void ChangeEnemyStateEnd(void);
	void ChangeEnemyStateWalk(void);
	void ChangeEnemyStateAction_1(void);
	void ChangeEnemyStateAction_2(void);
	void ChangeEnemyStateAction_3(void);
	void ChangeEnemyStateAction_4(void);
	void ChangeEnemyStateAction_5(void);
	void ChangeEnemyStateAction_6(void);
	void ChangeEnemyStateAction_Awake(void);
	void ChangeEnemyStateActionEndShort(void);
	void ChangeEnemyStateActionEndLong(void);


	// �J�ڏ���--------------------------------
	virtual const bool ConditionWalk(void);
	virtual const bool ConditionAttack_1(void);
	virtual const bool ConditionAttack_2(void);
	virtual const bool ConditionAttack_3(void);
	virtual const bool ConditionAttack_4(void);
	virtual const bool ConditionAttack_5(void);
	virtual const bool ConditionAttack_6(void);
	virtual const bool ConditionAttack_Awake(void);
	virtual const bool ConditionNearDistance(void);
	virtual const bool ConditionNearDisBack(void);
	virtual const bool ConditionMediumDistance(void);
	virtual const bool ConditionFarDistance(void);

	//�J�ڏ���::���O�ɂ��U��1�A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_1_Rag(void)const;
	//�J�ڏ���::���O�ɂ��U��2�A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_2_Rag(void)const;
	//�J�ڏ���::���O�ɂ��U��3�A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_3_Rag(void)const;
	//�J�ڏ���::���O�ɂ��U��4�A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_4_Rag(void)const;
	//�J�ڏ���::���O�ɂ��U��5�A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_5_Rag(void)const;
	//�J�ڏ���::���O�ɂ��U��6�A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_6_Rag(void)const;
	// �J�ڏ���::���O�ɂ��o���U���A�j���[�V�����̋N�����s����
	const bool ConditionEndAttack_Awake_Rag(void)const;
	// ----------------------------------------

};