#pragma once

//InputManager�̊g��
class InputManager;

class SimpleInput
{
public:

	//�L�[ID
	enum class SIMPLE
	{
		KEY_WEEKATC,//��U��
		KEY_STRONGATC,//���U��
		KEY_JUMP,//�W�����v
		KEY_DEFENCE,//�h��
		KEY_QUICKMOVE,//�����ړ�
		KEY_AVOID,//���
		KEY_SKILL_1,//�K�E1-2
		KEY_SKILL_2,//�K�E2-2
		KEY_UP,//�ړ�:��
		KEY_DOWN,//�ړ�:��
		KEY_LEFT,//�ړ�:��
		KEY_RIGHT,//�ړ�:�E
		MAX
	};


	//���͂���v���C���[
	enum class PL_NUMBER
	{
		PL_1,//�v���C���[1
		PL_2,//�v���C���[2
	};


	//�R���X�g���N�^
	SimpleInput(void);

	//�f�X�g���N�^
	~SimpleInput(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	void Update(const PL_NUMBER& number);

	// ���\�[�X�̔j��
	void Destroy(void);

	//�L�[�̏�Ԃ��擾
	bool GetSimpleKey(const SIMPLE& key)const;

private:

	//�o�^���Ă���L�[�̍X�V
	bool stateKey_[static_cast<int>(SIMPLE::MAX)];
	
};