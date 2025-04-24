#include "../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "Time.h"

#pragma region �萔�錾

// �g�[�^���^�C���̍ő�l
const float MAX_TOTAL_TIME_NUM = 99999999.0f;

// �������q
const float INIT_ZERO = 0.0f;

#pragma endregion

Time::Time(void):
	totalNowTime_(INIT_ZERO)
{
}

Time::~Time(void)
{
}

void Time::InitTotalNowTime(void)
{
	totalNowTime_ = INIT_ZERO;
}

void Time::SetTotalNowTime(const float time)
{
	totalNowTime_ = time;
}

void Time::UpdateTime(void)
{
	auto nowTime_ = SceneManager::GetInstance().GetDeltaTime();

	totalNowTime_ += nowTime_;
	if (MAX_TOTAL_TIME_NUM < totalNowTime_)totalNowTime_ = INIT_ZERO;
}

void Time::UpdateTime(const float speed)
{
	auto nowTime_ = SceneManager::GetInstance().GetDeltaTime() * speed;

	totalNowTime_ += nowTime_;
	if (MAX_TOTAL_TIME_NUM < totalNowTime_)totalNowTime_ = INIT_ZERO;
}

const float Time::GetTotalNowTime(void) const
{
	return totalNowTime_;
}

float& Time::GetTotalNowTimeReference(void)
{
	return totalNowTime_;
}
