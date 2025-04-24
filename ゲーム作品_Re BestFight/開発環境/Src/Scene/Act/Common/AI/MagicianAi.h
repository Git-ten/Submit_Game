#pragma once
#include "ActorAi.h"
class MagicianAi : public ActorAi
{

public:

	//コンストラクタ
	MagicianAi(std::weak_ptr<ActorBase> actor);

	//デストラクタ
	~MagicianAi(void)override;

private:

	//最低限の魔力
	static constexpr float MIN_MOVE_MAGIC = 10.0f;

	//コマンドアクション
	//弱攻撃1
	void ActionComandWeek_1(void)override;
	//弱攻撃2
	void ActionComandWeek_2(void)override;
	//弱攻撃3
	void ActionComandWeek_3(void)override;
	//弱攻撃4
	void ActionComandWeek_4(void)override;
	//弱攻撃5
	void ActionComandWeek_5(void)override;
	//弱攻撃6
	void ActionComandWeek_6(void)override;
	//強攻撃1
	void ActionComandStrong_1(void)override;
	//強攻撃2
	void ActionComandStrong_2(void)override;
	//強攻撃3
	void ActionComandStrong_3(void)override;
	//必殺技
	void ActionComandUnleashSkill(void)override;

};

