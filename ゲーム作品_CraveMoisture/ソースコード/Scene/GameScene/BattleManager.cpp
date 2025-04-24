#include "BattleManager.h"

BattleManager::BattleManager(void):
    battleStep_(0)
{
}

BattleManager::~BattleManager(void)
{
}

void BattleManager::Update(void)
{
    if (battleStep_ >= nextStepCondition_.size() - 1)return;

    if (nextStepCondition_[battleStep_]())
        battleStep_++;

}

void BattleManager::AddCondition(const int index, const std::function<bool(void)>& condition)
{
    if (index < 0)return;

    if (index >= nextStepCondition_.size())
    {
        int size = static_cast<int>(nextStepCondition_.size());
        nextStepCondition_.resize(size + (index - size) + 1);

        for (auto& condition : nextStepCondition_)
        {
            if (condition == nullptr)
                condition = []() { return true; };
        }
    }

    if (condition != nullptr)
        nextStepCondition_[index] = condition;
}

int BattleManager::GetNowBattleState(void)
{
    return battleStep_;
}

bool BattleManager::IsConditionTrue(const int index)
{
    if (index < 0)return false;
    if (index >= nextStepCondition_.size())return false;

    return nextStepCondition_[index]();
}