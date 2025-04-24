#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/Sound.h"
#include"../Manager/ResourceManager.h"
#include"../Application.h"
#include "StopScene.h"

#define MENYU ResourceManager::SRC::MENYU

StopScene::StopScene()
{
	nowSize_ = { 0, 0 };
	state_ = STATE::SELECT_MENYU;
	decisionState_ = STATE::SELECT_MENYU;
	shiftFlag_.emplace(STATE::SELECT_MENYU, false);
	shiftFlag_.emplace(STATE::RETURN_SELECT, false);
	shiftFlag_.emplace(STATE::RETURN_MENYU, false);
	shiftFlag_.emplace(STATE::ABOUT_GAME, false);
	imgMenyu_ = -1;
}

StopScene::~StopScene()
{
}

void StopScene::Init(void)
{
	nowSize_ = { MENYU_WSIZE.x, 0 };
	if (!Application::GetInstance().IsWidowMode())nowSize_.x = MENYU_FSIZE.x;

	state_ = STATE::FADE_IN;
	decisionState_ = STATE::SELECT_MENYU;

	//決定音
	decisionSod_ = SoundManager::GetInstance().Load(SoundManager::SOD::DECISION).lock();
	imgMenyu_ = ResourceManager::GetInstance().Load(MENYU).lock()->handleId_;
}

void StopScene::Update(void)
{
	if (state_ == STATE::FADE_OUT)
	{
		UpdateFadeOut();
	}
	else if (state_ == STATE::FADE_IN)
	{
		UpdateFadeIn();
	}
	else
	{
		UpdateSelect();
	}
}

void StopScene::Draw(void)
{
	DrawMenyu();
	//DrawMap();
}

void StopScene::Release(void)
{
}

StopScene::STATE StopScene::GetDecision(void) const
{
	return decisionState_;
}

void StopScene::SetDecision(const STATE& stat)
{
	decisionState_ = stat;
}

void StopScene::UpdateFadeOut(void)
{
	nowSize_.y -= SPEED;
	if (!Application::GetInstance().IsWidowMode())nowSize_.y -= SPEED / 3;

	if (nowSize_.y <= 0)
	{
		nowSize_.y = 0;
		state_ = STATE::SELECT_MENYU;
		decisionState_ = STATE::RETURN_MENYU;
	}
}

void StopScene::UpdateFadeIn(void)
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
		state_ = STATE::RETURN_SELECT;
	}
}

void StopScene::UpdateSelect(void)
{
	//マウス
	Position2D mousePos = InputManager::GetInstance().GetMousePos();
	mousePos = World2MapPos(mousePos);


	InputManager& ins = InputManager::GetInstance();
	if (Application::GetInstance().IsMouseFlag())
	{
		bool clickTrgUpFlag = ins.IsTrgUpMouseLeft();
		bool clickTrgDownFlag = ins.IsTrgDownMouseLeft();
		bool clickFlag = ins.IsClickMouseLeft();
		//カーソル処理-----------------------------------------
		if (RE_TITLE_MAP_POS.x - 1 <= mousePos.x && mousePos.x <= RE_TITLE_MAP_POS.x + 4)
		{
			if (mousePos.y == RE_TITLE_MAP_POS.y)
			{
				state_ = STATE::RETURN_SELECT;
				shiftFlag_.at(STATE::RETURN_MENYU) = false;
				shiftFlag_.at(STATE::ABOUT_GAME) = false;

				//決定処理
				if (clickTrgDownFlag)
				{
					shiftFlag_.at(STATE::RETURN_SELECT) = true;
				}
				if (clickTrgUpFlag)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					shiftFlag_.at(STATE::RETURN_SELECT) = false;
					decisionState_ = STATE::RETURN_SELECT;
				}
				if (clickFlag)
				{
					shiftFlag_.at(STATE::RETURN_SELECT) = true;
				}
			}
			else if (mousePos.y == RE_TITLE_MAP_POS.y + 1)
			{
				state_ = STATE::ABOUT_GAME;
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
				shiftFlag_.at(STATE::RETURN_MENYU) = false;

				//決定処理
				if (clickTrgDownFlag)
				{
					shiftFlag_.at(STATE::ABOUT_GAME) = true;
				}
				if (clickTrgUpFlag)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					shiftFlag_.at(STATE::ABOUT_GAME) = false;
					decisionState_ = STATE::ABOUT_GAME;
				}
				if (clickFlag)
				{
					shiftFlag_.at(STATE::ABOUT_GAME) = true;
				}
			}
			else if (mousePos.y == RE_TITLE_MAP_POS.y + 2)
			{
				state_ = STATE::RETURN_MENYU;
				shiftFlag_.at(STATE::ABOUT_GAME) = false;
				shiftFlag_.at(STATE::RETURN_SELECT) = false;

				//決定処理
				if (clickTrgDownFlag)
				{
					shiftFlag_.at(STATE::RETURN_MENYU) = true;
				}
				if (clickTrgUpFlag)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					shiftFlag_.at(STATE::RETURN_MENYU) = false;
					state_ = STATE::FADE_OUT;
				}
				if (clickFlag)
				{
					shiftFlag_.at(STATE::RETURN_MENYU) = true;
				}
			}
		}
		//キー入力操作
		else
		{
			//上方向
			if (ins.IsTrgDown(KEY_INPUT_W))
			{
				if (state_ == STATE::RETURN_MENYU)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					state_ = STATE::ABOUT_GAME;
					shiftFlag_.at(STATE::RETURN_SELECT) = false;
					shiftFlag_.at(STATE::RETURN_MENYU) = false;
				}
				else if (state_ == STATE::ABOUT_GAME)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					state_ = STATE::RETURN_SELECT;
					shiftFlag_.at(STATE::RETURN_MENYU) = false;
					shiftFlag_.at(STATE::ABOUT_GAME) = false;
				}
			}
			//下方向
			else if (ins.IsTrgDown(KEY_INPUT_S))
			{
				if (state_ == STATE::RETURN_SELECT)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					state_ = STATE::ABOUT_GAME;
					shiftFlag_.at(STATE::RETURN_SELECT) = false;
					shiftFlag_.at(STATE::RETURN_MENYU) = false;
				}
				else if (state_ == STATE::ABOUT_GAME)
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					state_ = STATE::RETURN_MENYU;
					shiftFlag_.at(STATE::RETURN_SELECT) = false;
					shiftFlag_.at(STATE::ABOUT_GAME) = false;
				}
			}

			//決定処理
			if (state_ == STATE::RETURN_SELECT)
			{
				if (ins.IsTrgUp(KEY_INPUT_RETURN))
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
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
			if (state_ == STATE::ABOUT_GAME)
			{
				if (ins.IsTrgUp(KEY_INPUT_RETURN))
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					shiftFlag_.at(STATE::ABOUT_GAME) = false;
					decisionState_ = STATE::ABOUT_GAME;
				}
				if (ins.IsTrgDown(KEY_INPUT_RETURN))
				{
					shiftFlag_.at(STATE::ABOUT_GAME) = true;
				}
				if (ins.IsNew(KEY_INPUT_RETURN))
				{
					shiftFlag_.at(STATE::ABOUT_GAME) = true;
				}
			}
			if (state_ == STATE::RETURN_MENYU)
			{
				if (ins.IsTrgUp(KEY_INPUT_RETURN))
				{
					decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
					shiftFlag_.at(STATE::RETURN_MENYU) = false;
					state_ = STATE::FADE_OUT;
				}
				if (ins.IsTrgDown(KEY_INPUT_RETURN))
				{
					shiftFlag_.at(STATE::RETURN_MENYU) = true;
				}
				if (ins.IsNew(KEY_INPUT_RETURN))
				{
					shiftFlag_.at(STATE::RETURN_MENYU) = true;
				}
			}
		}
		//-----------------------------------------------------
	}
	else
	{
		//コントローラ処理-------------------------------------
		auto controll_1 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		auto controll_2 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2);
		//上方向
		if ((controll_1.aKeyLY < 0 ||
			controll_2.aKeyLY < 0) &&
			(ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD1) || ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD2)))
		{
			if (state_ == STATE::RETURN_MENYU)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				state_ = STATE::ABOUT_GAME;
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
				shiftFlag_.at(STATE::RETURN_MENYU) = false;
			}
			else if (state_ == STATE::ABOUT_GAME)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				state_ = STATE::RETURN_SELECT;
				shiftFlag_.at(STATE::RETURN_MENYU) = false;
				shiftFlag_.at(STATE::ABOUT_GAME) = false;
			}
		}
		//下方向
		else if ((controll_1.aKeyLY > 0 ||
			controll_2.aKeyLY < 0) &&
			(ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD1) || ins.IsPadBtnTrgDownLa(InputManager::JOYPAD_NO::PAD2)))
		{
			if (state_ == STATE::RETURN_SELECT)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				state_ = STATE::ABOUT_GAME;
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
				shiftFlag_.at(STATE::RETURN_MENYU) = false;
			}
			else if (state_ == STATE::ABOUT_GAME)
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				state_ = STATE::RETURN_MENYU;
				shiftFlag_.at(STATE::RETURN_SELECT) = false;
				shiftFlag_.at(STATE::ABOUT_GAME) = false;
			}
		}

		//決定処理
		if (state_ == STATE::RETURN_SELECT)
		{
			if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
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
		if (state_ == STATE::ABOUT_GAME)
		{
			if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				shiftFlag_.at(STATE::ABOUT_GAME) = false;
				decisionState_ = STATE::ABOUT_GAME;
			}
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				shiftFlag_.at(STATE::ABOUT_GAME) = true;
			}
			if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				shiftFlag_.at(STATE::ABOUT_GAME) = true;
			}
		}
		if (state_ == STATE::RETURN_MENYU)
		{
			if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
				shiftFlag_.at(STATE::RETURN_MENYU) = false;
				state_ = STATE::FADE_OUT;
			}
			if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				shiftFlag_.at(STATE::RETURN_MENYU) = true;
			}
			if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) ||
				ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN))
			{
				shiftFlag_.at(STATE::RETURN_MENYU) = true;
			}
		}

		//Aボタンで強制的に戻る
		if (ins.IsTrgUp(KEY_INPUT_RETURN) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
			ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT))
		{
			decisionSod_.lock()->Play(DX_PLAYTYPE_BACK);
			shiftFlag_.at(STATE::RETURN_MENYU) = false;
			state_ = STATE::FADE_OUT;
		}

		//-----------------------------------------------------
	}
}

void StopScene::DrawMenyu(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	Position2D boxPos = { screenSize.x / 3 - 10, screenSize.y / 3 };

	unsigned int colorN = 0xffffff;
	unsigned int colorS = 0xff0000;

	//メニュー
	/*DrawExtendGraph(boxPos.x, boxPos.y,
		boxPos.x + nowSize_.x, boxPos.y + nowSize_.y,
		imgMenyu_, true);*/

	if (!Application::GetInstance().IsWidowMode())
	{
		DrawRectRotaGraph(boxPos.x + MENYU_FSIZE.x / 2 - 30, boxPos.y + MENYU_FSIZE.y / 2 + 100,
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

//	DrawBox(boxPos.x, boxPos.y, boxPos.x + nowSize_.x, boxPos.y + nowSize_.y, 0x000000, true);

	int fontSize = 50;
	Position2D pos = { RE_TITLE_MAP_POS.x, RE_TITLE_MAP_POS.y };
	Size2D mapSize = Application::GetInstance().GetMapSize();
	pos.x *= mapSize.x;
	pos.y *= mapSize.y;
	if (!Application::GetInstance().IsWidowMode())
	{
		fontSize = 100;
	}

	pos.x += fontSize * 2 - 30;

	if (state_ != STATE::FADE_IN && state_ != STATE::FADE_OUT)
	{
		SetFontSize(fontSize);
		if (state_ == STATE::RETURN_SELECT)
		{
			Position2D addPos = { 0, 0 };

			if (shiftFlag_.at(STATE::RETURN_SELECT))
			{
				addPos.x += 5;
				addPos.y += 5;
			}
			DrawFormatString(pos.x - 20 + addPos.x, pos.y + addPos.y, colorS, "Chara Select");
			DrawCircle(pos.x - fontSize / 2 - 10 + addPos.x, pos.y + fontSize / 3, fontSize / 3, 0x0000ff, true);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, colorN, "Chara Select");
		}
		pos = { RE_TITLE_MAP_POS.x, RE_TITLE_MAP_POS.y + 1 };
		pos.x *= mapSize.x;
		pos.y *= mapSize.y;

		pos.x += fontSize * 2 - 30;

		if (state_ == STATE::ABOUT_GAME)
		{
			Position2D addPos = { 0, 0 };

			if (shiftFlag_.at(STATE::ABOUT_GAME))
			{
				addPos.x += 5;
				addPos.y += 5;
			}
			DrawFormatString(pos.x - 20 + addPos.x, pos.y + addPos.y, colorS, "Controll");
			DrawCircle(pos.x - fontSize / 2 - 10 + addPos.x, pos.y + fontSize / 3, fontSize / 3, 0x0000ff, true);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, colorN, "Controll");
		}

		pos = { RE_TITLE_MAP_POS.x, RE_TITLE_MAP_POS.y + 2 };
		pos.x *= mapSize.x;
		pos.y *= mapSize.y;

		pos.x += fontSize * 2 - 30;

		if (state_ == STATE::RETURN_MENYU)
		{
			Position2D addPos = { 0, 0 };

			if (shiftFlag_.at(STATE::RETURN_MENYU))
			{
				addPos.x += 5;
				addPos.y += 5;
			}
			DrawFormatString(pos.x - 20 + addPos.x, pos.y + addPos.y, colorS, "Return");
			DrawCircle(pos.x - fontSize / 2 - 10 + addPos.x, pos.y + fontSize / 3, fontSize / 3, 0x0000ff, true);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, colorN, "Return");
		}
		SetFontSize(16);
	}
}
