#pragma once
class PlayerState
{
public:

	enum STATE
	{
		IDLE = 1 << 0,
		MOVE_UP = 1 << 1,
		MOVE_LEFT = 1 << 2,
		MOVE_DOWN = 1 << 3,
		MOVE_RIGHT = 1 << 4,
		MOVE = 1 << 5,
		DASH = 1 << 6,
		WALK = 1 << 7,
		ATTACK = 1 << 8,
		ATTACK_1 = 1 << 9,
		ATTACK_2 = 1 << 10,
		ATTACK_3 = 1 << 11,
		ATTACK_4 = 1 << 12,
		ATTACK_5 = 1 << 13,
		ATTACK_6 = 1 << 14,
		SKY_ATTACK_1 = 1 << 15,
		SKY_ATTACK_2 = 1 << 16,
		FALL_ATTACK = 1 << 17,
		CHARGE = 1 << 18,
		NORMAL_CHARGE = 1 << 19,
		FORWARD_CHARGE = 1 << 20,
		HORIZONTAL_CHARGE = 1 << 21,
		MIDDLE_ATTACK = 1 << 22,
		JUMP = 1 << 23,
		AVOID = 1 << 24,
		MIDDLE_AVOID = 1 << 25,
		TECHNIQUE = 1 << 26,
		SKILL = 1 << 27,
		HIT_LEFT = 1 << 28,
		HIT_RIGHT = 1 << 29,
		DEATH = 1 << 30,
		JUST_AVOID = 1 << 31,
	};
};