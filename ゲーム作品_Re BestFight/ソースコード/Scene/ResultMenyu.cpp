#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include"../Manager/ResourceManager.h"
#include"../Application.h"
#include "ResultMenyu.h"

#define MENYU ResourceManager::SRC::MENYU

ResultMenyu::ResultMenyu()
{
	nowSize_ = { 0, 0 };
	state_ = STATE::SELECT_MENYU;
	decisionState_ = STATE::SELECT_MENYU;
	shiftFlag_.emplace(STATE::RETURN_TITLE, false);
	shiftFlag_.emplace(STATE::RETURN_SELECT, false);
	imgMenyu_ = -1;
}

ResultMenyu::~ResultMenyu()
{
}

void ResultMenyu::Init(void)
{
	nowSize_ = { MENYU_WSIZE.x, 0 };
	if (!Application::GetInstance().IsWidowMode())nowSize_.x = MENYU_FSIZE.x;

	state_ = STATE::FADE_IN;
	decisionState_ = STATE::SELECT_MENYU;

	InitShader();

	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION).lock();
	imgMenyu_ = ResourceManager::GetInstance().Load(MENYU).lock()->handleId_;
}

void ResultMenyu::Update(void)
{
	if (state_ == STATE::FADE_IN)
	{
		UpdateFadeIn();
	}
	else
	{
		UpdateSelect();
	}
}

void ResultMenyu::Draw(void)
{
	DrawMenyu();
}

void ResultMenyu::Release(void)
{
}

ResultMenyu::STATE ResultMenyu::GetDecision(void) const
{
	return decisionState_;
}

void ResultMenyu::UpdateFadeIn(void)
{
	nowSize_.y += SPEED;

	int sizeY = MENYU_WSIZE.y;
	if (!Application::GetInstance().IsWidowMode())
	{
		nowSize_.y += SPEED / 3;
		sizeY = MENYU_FSIZE.y;
	}

	if (nowSize_.y >= sizeY)
	{
		nowSize_.y = sizeY;
		state_ = STATE::RETURN_TITLE;
	}
}

void ResultMenyu::UpdateSelect(void)
{
	//マウス
	Position2D mousePos = InputManager::GetInstance().GetMousePos();
	mousePos = World2MapPos(mousePos);


	InputManager& ins = InputManager::GetInstance();
	bool clickTrgUpFlag = ins.IsTrgUpMouseLeft();
	bool clickTrgDownFlag = ins.IsTrgDownMouseLeft();
	bool clickFlag = ins.IsClickMouseLeft();
	//カーソル処理-----------------------------------------
	if (TITLE_MAP_POS.x - 1 <= mousePos.x && mousePos.x <= TITLE_MAP_POS.x + 4)
	{
		if (mousePos.y == TITLE_MAP_POS.y)
		{
			state_ = STATE::RETURN_TITLE;
			shiftFlag_.at(STATE::RETURN_SELECT) = false;

			//決定処理
			if (clickTrgDownFlag)
			{
				shiftFlag_.at(STATE::RETURN_TITLE) = true;
			}
			if (clickTrgUpFlag)
			{
				shiftFlag_.at(STATE::RETURN_TITLE) = false;
				decisionState_ = STATE::RETURN_TITLE;
			}
			if (clickFlag)
			{
				shiftFlag_.at(STATE::RETURN_TITLE) = true;
			}
		}
		else if (mousePos.y == TITLE_MAP_POS.y + 1)
		{
			state_ = STATE::RETURN_SELECT;
			shiftFlag_.at(STATE::RETURN_TITLE) = false;

			//決定処理
			if (clickTrgDownFlag)
			{
				shiftFlag_.at(STATE::RETURN_SELECT) = true;
			}
			if (clickTrgUpFlag)
			{
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
				decisionState_ = STATE::RETURN_SELECT;
			}
			if (clickFlag)
			{
				shiftFlag_.at(STATE::RETURN_SELECT) = true;
			}
		}
	}
	//キー入力操作
	else
	{
		//上方向
		if (ins.IsTrgDown(KEY_INPUT_W))
		{
			if (state_ == STATE::RETURN_SELECT)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				state_ = STATE::RETURN_TITLE;
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
			}
		}
		//下方向
		else if (ins.IsTrgDown(KEY_INPUT_S))
		{
			if (state_ == STATE::RETURN_TITLE)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				state_ = STATE::RETURN_SELECT;
				shiftFlag_.at(STATE::RETURN_TITLE) = false;
			}
		}

		//決定処理
		if (state_ == STATE::RETURN_TITLE)
		{
			if (ins.IsTrgUp(KEY_INPUT_RETURN))
			{
				shiftFlag_.at(STATE::RETURN_TITLE) = false;
				decisionState_ = STATE::RETURN_TITLE;
			}
			if (ins.IsTrgDown(KEY_INPUT_RETURN))
			{
				shiftFlag_.at(STATE::RETURN_TITLE) = true;
			}
			if (ins.IsNew(KEY_INPUT_RETURN))
			{
				shiftFlag_.at(STATE::RETURN_TITLE) = true;
			}
		}
		if (state_ == STATE::RETURN_SELECT)
		{
			if (ins.IsTrgUp(KEY_INPUT_RETURN))
			{
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
				decisionState_ = STATE::RETURN_SELECT;
			}
			if (ins.IsTrgDown(KEY_INPUT_RETURN))
			{
				shiftFlag_.at(STATE::RETURN_SELECT) = true;
			}
			if (ins.IsNew(KEY_INPUT_RETURN))
			{
				shiftFlag_.at(STATE::RETURN_SELECT) = true;
			}
		}
	}
	//-----------------------------------------------------
	//コントローラ処理-------------------------------------
	auto controll_1 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	auto controll_2 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2);
	//上方向
	if ((controll_1.aKeyLY < 0 ||
		controll_2.aKeyLY < 0) &&
		(ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD1) || ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD2)))
	{
		if (state_ == STATE::RETURN_SELECT)
		{
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			state_ = STATE::RETURN_TITLE;
			shiftFlag_.at(STATE::RETURN_SELECT) = false;
		}
	}
	//下方向
	else if ((controll_1.aKeyLY > 0 ||
		controll_2.aKeyLY < 0) &&
		(ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD1) || ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD2)))
	{
		if (state_ == STATE::RETURN_TITLE)
		{
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			state_ = STATE::RETURN_SELECT;
			shiftFlag_.at(STATE::RETURN_TITLE) = false;
		}
	}

	//決定処理
	if (state_ == STATE::RETURN_TITLE)
	{
		if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			shiftFlag_.at(STATE::RETURN_TITLE) = false;
			decisionState_ = STATE::RETURN_TITLE;
		}
		if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			shiftFlag_.at(STATE::RETURN_TITLE) = true;
		}
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			shiftFlag_.at(STATE::RETURN_TITLE) = true;
		}
	}
	if (state_ == STATE::RETURN_SELECT)
	{
		if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			shiftFlag_.at(STATE::RETURN_SELECT) = false;
			decisionState_ = STATE::RETURN_SELECT;
		}
		if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			shiftFlag_.at(STATE::RETURN_SELECT) = true;
		}
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
		{
			shiftFlag_.at(STATE::RETURN_SELECT) = true;
		}
	}
	//-----------------------------------------------------
}

void ResultMenyu::DrawMenyu(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	Position2D boxPos = { screenSize.x / 3, screenSize.y / 3 };

	unsigned int colorN = 0xffffff;
	unsigned int colorS = 0xff0000;

	if (!Application::GetInstance().IsWidowMode())
	{
		DrawRectRotaGraph(boxPos.x + MENYU_FSIZE.x / 2, boxPos.y + MENYU_FSIZE.y / 2 + 100,
			0, MENYU_FSIZE.y / 2 - nowSize_.y / 2,
			nowSize_.x, nowSize_.y,
			2.0f, 0.0f, imgMenyu_, true);
	}
	else
	{
		DrawRectRotaGraph(boxPos.x + MENYU_WSIZE.x / 2, boxPos.y + MENYU_WSIZE.y / 2,
			0, MENYU_WSIZE.y / 2 - nowSize_.y / 2,
			nowSize_.x, nowSize_.y,
			1.0f, 0.0f, imgMenyu_, true);
	}


	int font = 50;
	if (!Application::GetInstance().IsWidowMode())font *= 2;

	if (state_ != STATE::FADE_IN)
	{
		Position2D pos = { TITLE_MAP_POS.x, TITLE_MAP_POS.y };
		Size2D mapSize = Application::GetInstance().GetMapSize();
		pos.x *= mapSize.x;
		pos.x -= 5;
		pos.y *= mapSize.y;

		SetFontSize(font);
		if (state_ == STATE::RETURN_TITLE)
		{
			Position2D addPos = { 0, 0 };

			if (shiftFlag_.at(STATE::RETURN_TITLE))
			{
				addPos.x += 5;
				addPos.y += 5;
			}
			DrawFormatString(pos.x - 10 + addPos.x, pos.y + addPos.y, colorS, "Title");
			DrawCircle(pos.x - 8 - font / 3 + addPos.x, pos.y + font / 3, font / 3, 0x0000ff, true);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, colorN, "Title");
		}

		pos = { TITLE_MAP_POS.x, TITLE_MAP_POS.y + 1 };
		pos.x *= mapSize.x;
		pos.x -= 5;
		pos.y *= mapSize.y;

		if (state_ == STATE::RETURN_SELECT)
		{
			Position2D addPos = { 0, 0 };

			if (shiftFlag_.at(STATE::RETURN_SELECT))
			{
				addPos.x += 5;
				addPos.y += 5;
			}
			DrawFormatString(pos.x - 10 + addPos.x, pos.y+ addPos.y, colorS, "Chara Select");
			DrawCircle(pos.x - 8 - font / 3 + addPos.x, pos.y + font / 3, font / 3, 0x0000ff, true);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, colorN, "Chara Select");
		}
		SetFontSize(16);
	}
}
