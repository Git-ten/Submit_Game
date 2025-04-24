#pragma once
#include "ActorAi.h"
class MagicianAi : public ActorAi
{

public:

	//�R���X�g���N�^
	MagicianAi(std::weak_ptr<ActorBase> actor);

	//�f�X�g���N�^
	~MagicianAi(void)override;

private:

	//�Œ���̖���
	static constexpr float MIN_MOVE_MAGIC = 10.0f;

	//�R�}���h�A�N�V����
	//��U��1
	void ActionComandWeek_1(void)override;
	//��U��2
	void ActionComandWeek_2(void)override;
	//��U��3
	void ActionComandWeek_3(void)override;
	//��U��4
	void ActionComandWeek_4(void)override;
	//��U��5
	void ActionComandWeek_5(void)override;
	//��U��6
	void ActionComandWeek_6(void)override;
	//���U��1
	void ActionComandStrong_1(void)override;
	//���U��2
	void ActionComandStrong_2(void)override;
	//���U��3
	void ActionComandStrong_3(void)override;
	//�K�E�Z
	void ActionComandUnleashSkill(void)override;

};

