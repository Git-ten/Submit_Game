#pragma once
#include "ActorAi.h"
class SordManAi : public ActorAi
{
public:

	//コンストラクタ
	SordManAi(std::weak_ptr<ActorBase> actor);

	//デストラクタ
	~SordManAi(void)override;

private:

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

