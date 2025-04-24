#include "MenyuBase.h"

MenyuBase::MenyuBase(void)
{
}

MenyuBase::~MenyuBase(void)
{
}

void MenyuBase::LoadSceneData(void)
{
}

void MenyuBase::SetMenyuIsOpen(bool isOpen)
{
	isGuiDraw_ = isOpen;
	InitOpenMenyu();
}

bool MenyuBase::IsOpen(void)
{
	return isGuiDraw_;
}

Position2D MenyuBase::GetPos(void)
{
	return Position2D();
}

void MenyuBase::InitSpecial(void)
{
}

void MenyuBase::UpdateSpecial(void)
{
}

void MenyuBase::InitOpenMenyu(void)
{
}

void MenyuBase::InitSoundPlayLaunch(void)
{
}

int MenyuBase::InputSelectMenyu(bool inputLeft, bool inputRight,
	int nowSelect,
	const int start, const int end)
{
	if (inputLeft)
	{
		nowSelect--;
		if (nowSelect <= start)
			nowSelect = end - 1;
	}
	if (inputRight)
	{
		nowSelect++;
		if (nowSelect >= end)
			nowSelect = start + 1;
	}

	return nowSelect;
}

void MenyuBase::DrawBack(void)
{
}

void MenyuBase::DrawSpecial(void)
{
}
