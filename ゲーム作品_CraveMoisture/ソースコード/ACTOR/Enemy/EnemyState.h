#pragma once
class EnemyState
{
public:

	enum STATE
	{
		NONE = 1 << 0,
		IDLE = 1 << 1,				// ‘Ò‹@
		WALK = 1 << 2,				// •à‚­
		ROAR = 1 << 3,				// ‹©‚Ñ
		ATTACK = 1 << 4,
		ATTACK_1 = 1 << 5,			// UŒ‚1
		ATTACK_2 = 1 << 6,			// UŒ‚2
		ATTACK_3 = 1 << 7,			// UŒ‚3
		ATTACK_4 = 1 << 8,			// UŒ‚4
		ATTACK_5 = 1 << 9,			// UŒ‚5
		ATTACK_6 = 1 << 10,			// UŒ‚6
		HIT_LEFT = 1 << 11,			// ‹¯‚Ý(¶)
		HIT_RIGHT = 1 << 12,		// ‹¯‚Ý(‰E)
		HIT_DOWN = 1 << 13,			// ƒ_ƒEƒ“
		GET_UP = 1 << 14,			// ‹N‚«ã‚ª‚é
		DEATH = 1 << 15,			// Ž€–S

		AWAKE = 1 << 16,			// ŠoÁ
		ATTACK_AWAKE = 1 << 17,		// ŠoÁUŒ‚
	};

};