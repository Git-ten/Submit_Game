#pragma once
#include "Application.h"

class ApplicationRun
{
public:

	//�ÓI�֐��Ń����j���O
	static void Running(const Application::ScreenStatus& status);

private:

	//�Q�[���̏I���𔻒肷��
	static void EndGame(bool& flg);

};