#pragma once
#include <chrono>

class DeltaTime
{
public:

	// コンストラクタ
	DeltaTime(void);
	// デストラクタ
	~DeltaTime(void);

	// 現在時間を更新
	// 更新前と現在のCPU時間とで差をとる
	void UpdateTime(void);

	// 現在時間を取得
	const float GetNowTime(void)const;

private:

	// デルタタイム(更新前)
	std::chrono::system_clock::time_point prevTime_;
	// デルタタイム(更新後)
	float nowTime_;
};

