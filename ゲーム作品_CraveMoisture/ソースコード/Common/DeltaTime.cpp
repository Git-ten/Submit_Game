#include "DeltaTime.h"

#pragma region íËêîêÈåæ

const float INIT_ZERO = 0.0f;

#pragma endregion

DeltaTime::DeltaTime(void) :
	prevTime_(std::chrono::system_clock::now()),
	nowTime_(INIT_ZERO)
{
}

DeltaTime::~DeltaTime(void)
{
}

void DeltaTime::UpdateTime(void)
{
	auto nowTime = std::chrono::system_clock::now();

	nowTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - prevTime_).count() / 1000000000.0);

	prevTime_ = nowTime;
}

const float DeltaTime::GetNowTime(void) const
{
	return nowTime_;
}