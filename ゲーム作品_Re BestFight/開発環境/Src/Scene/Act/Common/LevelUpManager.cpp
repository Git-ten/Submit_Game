#include"UserInterface.h"
#include "LevelUpManager.h"

LevelUpManager::LevelUpManager(std::weak_ptr<UserInterface>userInterface)
{
	userInterface_ = userInterface;
}

LevelUpManager::~LevelUpManager()
{
}

void LevelUpManager::Init()
{
	InitSpilitValues();
	InitMeetCondisions();
}

void LevelUpManager::InitSpilitValues()
{
	spilitValues_.clear();
	spilitValues_.emplace(LEVELUP_ID::ATTACK, [&]() {AttackExAllot();});
	spilitValues_.emplace(LEVELUP_ID::DEFENCE, [&]() {DefenceExAllot();});
	spilitValues_.emplace(LEVELUP_ID::AVOID, [&]() {AvoidExAllot();});
	spilitValues_.emplace(LEVELUP_ID::JUST, [&]() {JustExAllot();});
	spilitValues_.emplace(LEVELUP_ID::QUICK_MOVE, [&]() {QuickMoveExAllot();});
}

void LevelUpManager::InitMeetCondisions()
{
	meetCondisions_.clear();
	meetCondisions_.emplace(LEVELUP_ID::ATTACK, false);
	meetCondisions_.emplace(LEVELUP_ID::DEFENCE, false);
	meetCondisions_.emplace(LEVELUP_ID::AVOID, false);
	meetCondisions_.emplace(LEVELUP_ID::JUST, false);
	meetCondisions_.emplace(LEVELUP_ID::QUICK_MOVE, false);
}

void LevelUpManager::Update()
{
	for (auto& mCond : meetCondisions_)
	{
		if (mCond.second)
		{
			mCond.second = false;
			spilitValues_.at(mCond.first)();
		}
	}
}

void LevelUpManager::Release()
{
}

void LevelUpManager::MeetCondision(LEVELUP_ID id)
{
	const auto& ite = meetCondisions_.find(id);
	if (ite != meetCondisions_.end())
	{
		meetCondisions_.at(id) = true;
	}
}

void LevelUpManager::SpilitFunc()
{
}

void LevelUpManager::AttackExAllot(void)
{
	userInterface_.lock()->ChargeStateSkill(3.0f);
	userInterface_.lock()->ChargeStateAttackEx(15.0f);
}

void LevelUpManager::DefenceExAllot(void)
{
	userInterface_.lock()->ChargeStateSkill(3.0f);
	userInterface_.lock()->ChargeStateDefenceEx(10.0f);
}

void LevelUpManager::AvoidExAllot(void)
{
	userInterface_.lock()->ChargeStateSkill(3.0f);
	userInterface_.lock()->ChargeStateAvoidEx(10.0f);
}

void LevelUpManager::JustExAllot(void)
{
	userInterface_.lock()->ChargeStateSkill(10.0f);
	userInterface_.lock()->ChargeStateAttackEx(20.0f);
	userInterface_.lock()->ChargeStateDefenceEx(20.0f);
	userInterface_.lock()->ChargeStateAvoidEx(20.0f);
}

void LevelUpManager::QuickMoveExAllot(void)
{
	userInterface_.lock()->ChargeStateSkill(3.0f);
	userInterface_.lock()->ChargeStateAvoidEx(30.0f);
}