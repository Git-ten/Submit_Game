#include<DxLib.h>
#include"../../Application.h"
#include"../../Utility/VectorHeader.h"
#include"../SceneManager.h"
#include "Fade.h"

Fade::Fade()
{
	IsFadeFlag_ = false;
	IsEndFade_ = true;
	IsManualFlag_ = false;
	speed_ = DEFAULT_FADE_SPEED;
	state_ = FADE_STATE::NONE;
	fadeUpdate_ = [&]() {UpdateFadeNone(); };
}

Fade::~Fade()
{
}

void Fade::Update()
{
	if (IsEndFade_ && !IsFadeFlag_)return;

	fadeUpdate_();
}

void Fade::Draw()
{
	if (IsEndFade_ && !IsFadeFlag_)return;

	//ï`âÊèàóù
	Size2D size = Application::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
	DrawBox(0, 0, size.x, size.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fade::FadeStart(void)
{
	IsFadeFlag_ = true;
	IsEndFade_ = true;
	IsManualFlag_ = false;
	ChangeFadeState(FADE_STATE::FADE_OUT);
}

void Fade::FadeStart(const FADE_STATE& state)
{
	IsFadeFlag_ = true;
	IsEndFade_ = false;
	IsManualFlag_ = true;
	ChangeFadeState(state);
}

void Fade::ChangeFadeState(const FADE_STATE& state)
{
	state_ = state;

	switch (state_)
	{
	case FADE_STATE::FADE_IN:
		fadeUpdate_ = [&]() {UpdateFadeIn(); };
		alpha_ = ALPHA_MAX + 1.0f;
		IsFadeFlag_ = true;
		break;
	case FADE_STATE::FADE_OUT:
		fadeUpdate_ = [&]() {UpdateFadeOut(); };
		alpha_ = ALPHA_MIN - 1.0f;
		IsFadeFlag_ = true;
		break;
	default:
		fadeUpdate_ = [&]() {UpdateFadeNone(); };
		alpha_ = 0.0f;
		IsFadeFlag_ = false;
		break;
	}
}

void Fade::SetFadeSpeed(float speed)
{
	speed_ = speed;
}

void Fade::DefaultFadeSpeed()
{
	speed_ = DEFAULT_FADE_SPEED;
}

bool Fade::IsFadeFlag(void)
{
	return IsFadeFlag_;
}

void Fade::SetIsEndFlag(bool flg)
{
	IsEndFade_ = flg;
}

bool Fade::IsEndFlag(void)
{
	return IsEndFade_;
}

Fade::FADE_STATE Fade::GetNowState(void) const
{
	return state_;
}

void Fade::UpdateFadeNone(void)
{
	return;
}

void Fade::UpdateFadeOut(void)
{
	if (alpha_ < ALPHA_MAX)
	{
		alpha_+= DEFAULT_FADE_SPEED;
	}
	else
	{
		alpha_ = ALPHA_MAX;
		if (IsManualFlag_)
		{
			IsFadeFlag_ = false;
			ChangeFadeState(FADE_STATE::NONE);
			return;
		}

		ChangeFadeState(FADE_STATE::FADE_IN);
	}
}

void Fade::UpdateFadeIn(void)
{
	if (alpha_ > ALPHA_MIN)
	{
		alpha_-= DEFAULT_FADE_SPEED;
	}
	else
	{
		alpha_ = ALPHA_MIN;
		IsFadeFlag_ = false;
	/*	if (IsManualFlag_)
		{
			ChangeFadeState(FADE_STATE::NONE);
			return;
		}*/
		//ChangeFadeState(FADE_STATE::FADE_OUT);
		ChangeFadeState(FADE_STATE::NONE);
	}
}
