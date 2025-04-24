#pragma once
#include<memory>
#include<map>
#include<functional>

class UserInterface;

class LevelUpManager
{
public:

	//�グ��p�����[�^��ID
	//�e�p�����[�^�̏オ����ID�ŕ�����
	enum class LEVELUP_ID
	{
		ATTACK,
		DEFENCE,
		AVOID,
		JUST,
		QUICK_MOVE,
	};

	//�R���X�g���N�^
	LevelUpManager(std::weak_ptr<UserInterface>userInterface);
	//�f�X�g���N�^
	~LevelUpManager(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�������J��
	void Release(void);

	//�����𖞂��������x���A�b�vID���Z�b�g
	void MeetCondision(LEVELUP_ID id);

private:

	using SplitFuncsData = std::map<LEVELUP_ID, std::function<void(void)>>;

	//���x�����Ƃɕ������ė^����o���l�v�Z
	SplitFuncsData spilitValues_;
	//���x���A�b�v�����𖞂�������
	std::map<LEVELUP_ID, bool>meetCondisions_;
	
	//���L����UI�f�[�^
	std::weak_ptr<UserInterface>userInterface_;


	//�����ł����g��Ȃ��֐�
	void InitSpilitValues(void);
	void InitMeetCondisions(void);
	//�������p�̉������Ȃ��֐�
	void SpilitFunc();
	//�U���ɂ��o���l�̊���U��
	void AttackExAllot(void);
	//�h��ɂ��o���l�̊���U��
	void DefenceExAllot(void);
	//����ɂ��o���l�̊���U��
	void AvoidExAllot(void);
	//�W���X�g����ɂ��o���l�̊���U��
	void JustExAllot(void);
	//�����ړ��ɂ��o���l�̊���U��
	void QuickMoveExAllot(void);

};