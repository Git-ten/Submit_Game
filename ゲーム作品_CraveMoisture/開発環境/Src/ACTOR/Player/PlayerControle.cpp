#include "../../Application/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "PlayerControle.h"

#pragma region íËêîêÈåæ

const float CHARGE_LIMIT_TIME_FOR_HOR = 0.15f;
const float CHARGE_MAX_TIME = 0.3f;

#pragma endregion

PlayerControle::PlayerControle(PlayerData& data, PlayerState::STATE& state):
	playerData_(&data),
	playerState_(&state)
{
}

PlayerControle::~PlayerControle(void)
{
}

void PlayerControle::Init(void)
{
}

void PlayerControle::Update(void)
{
	InputPlayerControle();
}

void PlayerControle::InputPlayerControle(void)
{
	if (*playerState_ & PlayerState::DEATH||
		*playerState_ & PlayerState::HIT_LEFT ||
		*playerState_ & PlayerState::HIT_RIGHT ||
		*playerState_ & PlayerState::WALK)return;

	InputPlayerMove();
	InputPlayerJump();
	InputPlayerAvoid();
	InputPlayerDash();
	InputPlayerAttack();
	InputPlayerTechnique();
	InputPlayerSkill();
}

void PlayerControle::InputPlayerMove(void)
{
	if (*playerState_ & PlayerState::ATTACK)return;
	if (*playerState_ & PlayerState::JUMP)return;
	if (*playerState_ & PlayerState::AVOID)return;
	if (*playerState_ & PlayerState::TECHNIQUE)return;
	if (*playerState_ & PlayerState::SKILL)return;

	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("moveForward"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::MOVE | PlayerState::MOVE_UP);
	if (input.GetInput("moveLeft"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::MOVE | PlayerState::MOVE_LEFT);
	if (input.GetInput("moveBack"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::MOVE | PlayerState::MOVE_DOWN);
	if (input.GetInput("moveRight"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::MOVE | PlayerState::MOVE_RIGHT);
}

void PlayerControle::InputPlayerDash(void)
{
	if (*playerState_ & PlayerState::JUMP)return;
	if (*playerState_ & PlayerState::TECHNIQUE)return;


	if (*playerState_ & PlayerState::STATE::MIDDLE_AVOID)
	{
		if(*playerState_ & PlayerState::STATE::MOVE)
			*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::DASH);
	}
}

void PlayerControle::InputPlayerAttack(void)
{
	if (*playerState_ & PlayerState::TECHNIQUE)return;
	if (*playerState_ & PlayerState::SKILL)return;
	if (*playerState_ & PlayerState::CHARGE)
	{
		chargeTime_.InitTotalNowTime();
		return;
	}

	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("normalAttack"))
	{
		if (*playerState_ & PlayerState::JUMP &&
			!(*playerState_ & PlayerState::MIDDLE_ATTACK) &&
			!(*playerState_ & PlayerState::ATTACK))
			*playerState_ = static_cast<PlayerState::STATE>(PlayerState::JUMP |
				PlayerState::ATTACK | PlayerState::SKY_ATTACK_1);

		else if (*playerState_ & PlayerState::MIDDLE_ATTACK)
			*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::ATTACK);

		else if(!(*playerState_ & PlayerState::ATTACK))
			*playerState_ = static_cast<PlayerState::STATE>(PlayerState::ATTACK | PlayerState::ATTACK_1);
	}
	else if (input.GetInput("charge"))
	{
		if(*playerState_ & PlayerState::JUST_AVOID)
			*playerState_ = static_cast<PlayerState::STATE>(PlayerState::STATE::ATTACK |
				PlayerState::CHARGE | PlayerState::NORMAL_CHARGE);

		if (chargeTime_.GetTotalNowTime() >= CHARGE_LIMIT_TIME_FOR_HOR)
		{
			if (input.GetInput("moveForward"))
				*playerState_ = static_cast<PlayerState::STATE>(PlayerState::STATE::ATTACK | 
					PlayerState::CHARGE | PlayerState::FORWARD_CHARGE);
			else if (input.GetInput("moveLeft") || input.GetInput("moveRight"))
				*playerState_ = static_cast<PlayerState::STATE>(PlayerState::STATE::ATTACK | 
					PlayerState::CHARGE | PlayerState::HORIZONTAL_CHARGE);
		}

		if (chargeTime_.GetTotalNowTime() >= CHARGE_MAX_TIME)
			*playerState_ = static_cast<PlayerState::STATE>(PlayerState::STATE::ATTACK | 
				PlayerState::CHARGE | PlayerState::NORMAL_CHARGE);

		chargeTime_.UpdateTime();
	}
	else
	{
		chargeTime_.InitTotalNowTime();
	}
}

void PlayerControle::InputPlayerJump(void)
{
	if (*playerState_ & PlayerState::ATTACK)return;
	if (*playerState_ & PlayerState::AVOID)return;
	if (*playerState_ & PlayerState::TECHNIQUE)return;
	if (*playerState_ & PlayerState::SKILL)return;

	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("jump"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::JUMP);
}

void PlayerControle::InputPlayerAvoid(void)
{
	if (*playerState_ & PlayerState::ATTACK)return;
	if (*playerState_ & PlayerState::TECHNIQUE)return;
	if (*playerState_ & PlayerState::SKILL)return;

	auto& input = SimpleInput::GetInstance();
	if(input.GetInput("avoid"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::AVOID);
}

void PlayerControle::InputPlayerTechnique(void)
{
	if (*playerState_ & PlayerState::JUMP)return;
	if (*playerState_ & PlayerState::AVOID)return;
	if (*playerState_ & PlayerState::ATTACK)return;
	if (*playerState_ & PlayerState::SKILL)return;

	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("technique"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::TECHNIQUE);
}

void PlayerControle::InputPlayerSkill(void)
{
	if (*playerState_ & PlayerState::TECHNIQUE)return;
	if (*playerState_ & PlayerState::JUMP)return;
	if (*playerState_ & PlayerState::AVOID)return;
	if (*playerState_ & PlayerState::ATTACK)return;

	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("skill"))
		*playerState_ = static_cast<PlayerState::STATE>(*playerState_ | PlayerState::SKILL);
}