#pragma once
#include "Application.h"

class ApplicationRun
{
public:

	//静的関数でランニング
	static void Running(const Application::ScreenStatus& status);

private:

	//ゲームの終了を判定する
	static void EndGame(bool& flg);

};