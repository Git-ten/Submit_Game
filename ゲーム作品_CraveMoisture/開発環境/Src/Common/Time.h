#pragma once
#include <chrono>

class Time
{
public:

	// コンストラクタ
	Time(void);
	// デストラクタ
	~Time(void);

	// 総時間を初期化する
	void InitTotalNowTime(void);

	// 総時間を強制的に引数の値にする
	void SetTotalNowTime(const float time);

	// 現在時間を更新
	// 更新前と現在のCPU時間とで差をとる
	void UpdateTime(void);
	void UpdateTime(const float speed);

	// 総時間を取得
	const float GetTotalNowTime(void)const;
	// 総時間の参照を取得
	float& GetTotalNowTimeReference(void);

private:

	// 総タイム
	float totalNowTime_;

};
