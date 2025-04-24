#pragma once
class EnemyState
{
public:

	enum STATE
	{
		NONE = 1 << 0,
		IDLE = 1 << 1,				// �ҋ@
		WALK = 1 << 2,				// ����
		ROAR = 1 << 3,				// ����
		ATTACK = 1 << 4,
		ATTACK_1 = 1 << 5,			// �U��1
		ATTACK_2 = 1 << 6,			// �U��2
		ATTACK_3 = 1 << 7,			// �U��3
		ATTACK_4 = 1 << 8,			// �U��4
		ATTACK_5 = 1 << 9,			// �U��5
		ATTACK_6 = 1 << 10,			// �U��6
		HIT_LEFT = 1 << 11,			// ����(��)
		HIT_RIGHT = 1 << 12,		// ����(�E)
		HIT_DOWN = 1 << 13,			// �_�E��
		GET_UP = 1 << 14,			// �N���オ��
		DEATH = 1 << 15,			// ���S

		AWAKE = 1 << 16,			// �o��
		ATTACK_AWAKE = 1 << 17,		// �o���U��
	};

};