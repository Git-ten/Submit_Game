#pragma once
#include <memory>
#include<functional>
#include "../../Common/Time.h"
#include "PlayerState.h"
#include "PlayerData.h"
#include "PlayerWeapon/PlayerWeaponBase.h"
#include "../ActorData.h"


class PlayerControle
{
public:

	PlayerControle(PlayerData& data, PlayerState::STATE& state);

	~PlayerControle(void);

	void Init(void);
	void Update(void);

private:

	PlayerData* playerData_;
	PlayerState::STATE* playerState_;

	Time chargeTime_;

	// èâä˙âªä÷êî
	void InputPlayerControle(void);
	void InputPlayerMove(void);
	void InputPlayerDash(void);
	void InputPlayerAttack(void);
	void InputPlayerJump(void);
	void InputPlayerAvoid(void);
	void InputPlayerTechnique(void);
	void InputPlayerSkill(void);
};