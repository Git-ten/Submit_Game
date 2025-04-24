#pragma once
#include <functional>
#include <vector>

class BattleManager
{
public:

	BattleManager(void);
	~BattleManager(void);

	void Update(void);

	void AddCondition(const int index, const std::function<bool(void)>& condition);

	int GetNowBattleState(void);

	bool IsConditionTrue(const int index);

private:

	std::vector<std::function<bool(void)>>nextStepCondition_;
	int battleStep_;

};