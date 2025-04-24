#include <DxLib.h>
#include "../Application/Application.h"
#include "Fader.h"

#pragma region �萔�錾

// �t�F�C�h�̑��x
const float DEFAULT_FADE_SPEED = 10.0f;
// �Ó]���̍ő�A���t�@�l
const float ALPHA_MAX = 255.0f;
// ���]���̍ŏ��A���t�@�l
const float ALPHA_MIN = 0.0f;

// �t�F�C�h���s���Ƃ��̍�����W
const Position2D FADE_LEFT_POS = { 0, 0 };

#pragma endregion

Fader::Fader(void):
	nowState_(FADER_STATE::NONE),
	faderUpdate_([&]() { UpdateFadeNone(); }),
	alpha_(ALPHA_MIN)
{
	const Size2D& screenSize = Application::GetInstance().GetScreenSize();
	fadeBackTexture_ = MakeScreen(screenSize.x, screenSize.y);
}

Fader::~Fader(void)
{
	DeleteGraph(fadeBackTexture_);
}

void Fader::Update(void)
{
	faderUpdate_();
}

void Fader::Draw(void)
{
	if (nowState_ == FADER_STATE::NONE)return;

	// �Ó]���̔w�i
	DrawFadeBack();

	const auto& size = Application::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
	DrawBox(FADE_LEFT_POS.x, FADE_LEFT_POS.y, size.x, size.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast<int>(ALPHA_MIN));
}

void Fader::FadeStart(void)
{
	ChangeFadeState(FADER_STATE::FADE_OUT);
	SetDrawScreen(fadeBackTexture_);
	// ��ʂ��N���A
	ClearDrawScreen();

	const Size2D& deskTopSize = Application::GetInstance().GetScreenDesktopSize();
	int mainScreen = Application::GetInstance().GetMainScreen();

	DrawGraph(FADE_LEFT_POS.x, FADE_LEFT_POS.y, mainScreen, true);
	
	SetDrawScreen(mainScreen);
}

const bool Fader::IsScreenBrack(void) const
{
	return alpha_ >= ALPHA_MAX;
}

const bool Fader::IsFadeOut(void) const
{
	return nowState_ == FADER_STATE::FADE_OUT;
}

const bool Fader::IsFadeEnd(void) const
{
	return nowState_ == FADER_STATE::NONE;
}

void Fader::ChangeFadeState(const FADER_STATE& state)
{
	nowState_ = state;

	switch (nowState_)
	{
	case FADER_STATE::FADE_IN:
		faderUpdate_ = [&]() { UpdateFadeIn(); };
		break;
	case FADER_STATE::FADE_OUT:
		faderUpdate_ = [&]() { UpdateFadeOut(); };
		break;
	default:
		faderUpdate_ = [&]() { UpdateFadeNone();  };
		break;
	}
}

void Fader::DrawFadeBack(void)
{
	if (nowState_ != FADER_STATE::FADE_OUT)return;

	DrawGraph(FADE_LEFT_POS.x, FADE_LEFT_POS.y, fadeBackTexture_, true);
}

void Fader::UpdateFadeNone(void)
{
}

void Fader::UpdateFadeOut(void)
{
	if (alpha_ < ALPHA_MAX)
	{
		alpha_ += DEFAULT_FADE_SPEED;
	}
	else
	{
		alpha_ = ALPHA_MAX;
		ChangeFadeState(FADER_STATE::FADE_IN);
	}
}

void Fader::UpdateFadeIn(void)
{
	if (alpha_ > ALPHA_MIN)
	{
		alpha_ -= DEFAULT_FADE_SPEED;
	}
	else
	{
		alpha_ = ALPHA_MIN;
		ChangeFadeState(FADER_STATE::NONE);
	}
}