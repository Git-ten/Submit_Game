#include <cassert>
#include <DxLib.h>
#include "SimpleInput.h"
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

#pragma region 定数宣言

const int POV_TOP = 0;
const int POV_TOP_RIGHT = 4500;
const int POV_TOP_LEFT = 31500;
const int POV_LEFT = 27000;
const int POV_DOWN_LEFT = 22500;
const int POV_DOWN = 18000;
const int POV_DOWN_RIGHT = 13500;
const int POV_RIGHT = 9000;

// 例外子
const int EXCEPTION_VALUE = -1;

// 符号判定用
const int DECISION_VALUE = 0;
#pragma endregion

void InputManager::CreateInstance(void)
{
	assert(instance_ == nullptr || !"InputManagerが作成されているのに再び生成しようとしています");

	instance_ = new InputManager();
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	return *instance_;
}

void InputManager::Init(void)
{

	// ゲームで使用したいキーを、
	// 事前にここで登録しておいてください
	InputManager::GetInstance().Add(KEY_INPUT_W);
	InputManager::GetInstance().Add(KEY_INPUT_A);
	InputManager::GetInstance().Add(KEY_INPUT_S);
	InputManager::GetInstance().Add(KEY_INPUT_D);
	InputManager::GetInstance().Add(KEY_INPUT_SPACE);
	InputManager::GetInstance().Add(KEY_INPUT_ESCAPE);
	InputManager::GetInstance().Add(KEY_INPUT_Q);
	InputManager::GetInstance().Add(KEY_INPUT_R);
	InputManager::GetInstance().Add(KEY_INPUT_RETURN);

	InputManager::MouseInfo info;

	// 左クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_LEFT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

	// 右クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_RIGHT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

	//中央クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_MIDDLE;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

	SetUseJoypadVibrationFlag(true);

	// シンプルインプット
	InitSimpleInput();
}

void InputManager::Update(void)
{

	// キーボード検知
	for (auto& p : keyInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// マウス検知
	mouseInput_ = GetMouseInput();
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	for (auto& p : mouseInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = mouseInput_ == p.second.key;
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// パッド情報
	SetJPadInState(JOYPAD_NO::KEY_PAD1);
	SetJPadInState(JOYPAD_NO::PAD1);
	SetJPadInState(JOYPAD_NO::PAD2);
	SetJPadInState(JOYPAD_NO::PAD3);
	SetJPadInState(JOYPAD_NO::PAD4);

}

void InputManager::Destroy(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
	SimpleInput::GetInstance().Destroy();
	delete instance_;
	instance_ = nullptr;
}

void InputManager::Add(int key)
{
	InputManager::Info info = InputManager::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	keyInfos_.emplace(key, info);
}

const bool InputManager::IsNew(int key) const
{
	return Find(key).keyNew;
}

const bool InputManager::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

const bool InputManager::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

const Position2D& InputManager::GetMousePos(void) const
{
	return mousePos_;
}

void InputManager::SetMousePos(Position2D pos)
{
	mousePos_ = pos;
	SetMousePoint(mousePos_.x, mousePos_.y);
}

const int InputManager::GetMouse(void) const
{
	return mouseInput_;
}

const bool InputManager::IsClickMouseLeft(void) const
{
	return mouseInput_ == MOUSE_INPUT_LEFT;
}

const bool InputManager::IsClickMouseRight(void) const
{
	return mouseInput_ == MOUSE_INPUT_RIGHT;
}

const bool InputManager::IsClickMouseMiddle(void) const
{
	return mouseInput_ == MOUSE_INPUT_MIDDLE;
}

const bool InputManager::IsTrgDownMouseLeft(void) const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown;
}

const bool InputManager::IsTrgUpMouseLeft(void)const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgUp;
}

const bool InputManager::IsTrgDownMouseRight(void) const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown;
}

const bool InputManager::IsTrgUpMouseRight(void)const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgUp;
}

const bool InputManager::IsTrgDownMouseMiddle(void) const
{
	return FindMouse(MOUSE_INPUT_MIDDLE).keyTrgDown;
}

const bool InputManager::IsTrgUpMouseMiddle(void) const
{
	return FindMouse(MOUSE_INPUT_MIDDLE).keyTrgUp;
}

InputManager::JOYPAD_IN_STATE InputManager::GetJPadInputState(const JOYPAD_NO& no)
{

	JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

	auto type = GetJPadType(no);

	switch (type)
	{
	case InputManager::JOYPAD_TYPE::OTHER:
		break;
	case InputManager::JOYPAD_TYPE::XBOX_360:
	{
	}
	break;
	case InputManager::JOYPAD_TYPE::XBOX_ONE:
	{

		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//L     R
		//   Y
		// X   B
		//   A

		idx = static_cast<int>(JOYPAD_BTN::R_STICK_BUTTON);
		ret.buttonsNew[idx] = d.Buttons[9];

		idx = static_cast<int>(JOYPAD_BTN::MENYU_TRIGGER);
		ret.buttonsNew[idx] = d.Buttons[7];

		idx = static_cast<int>(JOYPAD_BTN::R);
		ret.buttonsNew[idx] = d.Buttons[5];// R

		idx = static_cast<int>(JOYPAD_BTN::L);
		ret.buttonsNew[idx] = d.Buttons[4];// L

		idx = static_cast<int>(JOYPAD_BTN::TOP);
		ret.buttonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.buttonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.buttonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.buttonsNew[idx] = d.Buttons[0];// A

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.buttonsNew[idx] = x.RightTrigger;// R_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.buttonsNew[idx] = x.LeftTrigger; // L_TRIGGER




		idx = static_cast<int>(JOYPAD_BTN::L_POV_LEFT);
		ret.buttonsNew[idx] = IsPadBtnPovNewLeft(no); // L_LEFT_POV

		idx = static_cast<int>(JOYPAD_BTN::L_POV_RIGHT);
		ret.buttonsNew[idx] = IsPadBtnPovNewRight(no); // L_RIGHT_POV

		idx = static_cast<int>(JOYPAD_BTN::L_POV_TOP);
		ret.buttonsNew[idx] = IsPadBtnPovNewTop(no); // L_TOP_POV

		idx = static_cast<int>(JOYPAD_BTN::L_POV_DOWN);
		ret.buttonsNew[idx] = IsPadBtnPovNewDown(no); // L_DOWN_POV

		// 左スティック
		ret.aKeyLX = d.X;
		ret.aKeyLY = d.Y;

		// 右スティック
		ret.aKeyRX = d.Rx;
		ret.aKeyRY = d.Ry;

	}
	break;
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_4:
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SENSE:
	{

		auto d = GetJPadDInputState(no);
		int idx;

		//   △
		// □  〇
		//   ×

		idx = static_cast<int>(JOYPAD_BTN::TOP);
		ret.buttonsNew[idx] = d.Buttons[3];// △

		idx = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.buttonsNew[idx] = d.Buttons[0];// □

		idx = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.buttonsNew[idx] = d.Buttons[2];// 〇

		idx = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.buttonsNew[idx] = d.Buttons[1];// ×

		// 左スティック
		ret.aKeyLX = d.X;
		ret.aKeyLY = d.Y;

		// 右スティック
		ret.aKeyRX = d.Z;
		ret.aKeyRY = d.Rz;

	}
	break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_L:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_R:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_PRO_CTRL:
		break;
	case InputManager::JOYPAD_TYPE::MAX:
		break;
	}

	return ret;

}

const bool InputManager::IsAllKey(void)
{
	if (IsTrgDownMouseLeft())return true;

	for (const auto& key : keyInfos_)
	{
		if (IsTrgDown(key.first))
			return true;
	}

	return false;
}

const bool InputManager::IsAllPadBtnInput(void)
{
	// パッドがないなら、強制的にfalseを返す
	if (GetJPadNum() <= 0)return false;

	const JOYPAD_NO padNumber = JOYPAD_NO::PAD1;

	for (int btn = 0; btn < static_cast<int>(JOYPAD_BTN::MAX); btn++)
	{
		if (padInfos_[static_cast<int>(padNumber)].isTrgDown[btn])
		{
			return true;
		}
	}
	return false;
}

int InputManager::GetJPadNum(void)
{
	return GetJoypadNum();
}

const bool InputManager::IsPadBtnNew(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const
{
	return padInfos_[static_cast<int>(no)].isNew[static_cast<int>(btn)];
}

const bool InputManager::IsPadBtnTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const
{
	return padInfos_[static_cast<int>(no)].isTrgDown[static_cast<int>(btn)];
}

const bool InputManager::IsPadBtnTrgUp(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const
{
	return padInfos_[static_cast<int>(no)].isTrgUp[static_cast<int>(btn)];
}

const bool InputManager::IsPadBtnTrgDownLa(const JOYPAD_NO& no) const
{
	return padInfos_[static_cast<int>(no)].isTrgDownLa;
}

const bool InputManager::IsPadBtnTrgDownRa(const JOYPAD_NO& no) const
{
	return padInfos_[static_cast<int>(no)].isTrgDownRa;
}

const bool InputManager::IsPadBtnPovNewTop(const JOYPAD_NO& no)
{
	auto d = GetJPadDInputState(no);
	int povNo = static_cast<int>(no) - static_cast<int>(JOYPAD_NO::PAD1);
	int pov = d.POV[povNo];

	if (pov == POV_TOP)
	{
		// 上
		return true;
	}
	else if (pov == POV_TOP_RIGHT)
	{
		// 右上
		return true;
	}
	else if (pov == POV_TOP_LEFT)
	{
		// 左上
		return true;
	}
	return false;
}

const bool InputManager::IsPadBtnPovTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn)
{
	return padInfos_[static_cast<int>(no)].isTrgDown[static_cast<int>(btn)];
}

const bool InputManager::IsPadBtnPovNewLeft(const JOYPAD_NO& no)
{
	auto d = GetJPadDInputState(no);
	int povNo = static_cast<int>(no) - static_cast<int>(JOYPAD_NO::PAD1);
	int pov = d.POV[povNo];

	if (pov == POV_LEFT)
	{
		// 左
		return true;
	}
	else if (pov == POV_TOP_LEFT)
	{
		// 左上
		return true;
	}
	else if (pov == POV_DOWN_LEFT)
	{
		// 左下
		return true;
	}
	return false;

}

const bool InputManager::IsPadBtnPovNewDown(const JOYPAD_NO& no)
{
	auto d = GetJPadDInputState(no);
	int povNo = static_cast<int>(no) - static_cast<int>(JOYPAD_NO::PAD1);
	int pov = d.POV[povNo];

	if (pov == POV_DOWN)
	{
		// 下
		return true;
	}
	else if (pov == POV_DOWN_RIGHT)
	{
		// 右下
		return true;
	}
	else if (pov == POV_DOWN_LEFT)
	{
		// 左下
		return true;
	}
	return false;
}

const bool InputManager::IsPadBtnPovNewRight(const JOYPAD_NO& no)
{
	auto d = GetJPadDInputState(no);
	int povNo = static_cast<int>(no) - static_cast<int>(JOYPAD_NO::PAD1);
	int pov = d.POV[povNo];

	if (pov == POV_RIGHT)
	{
		// 右
		return true;
	}
	else if (pov == POV_TOP_RIGHT)
	{
		// 右上
		return true;
	}
	else if (pov == POV_DOWN_RIGHT)
	{
		// 右下
		return true;
	}
	return false;
}

const bool InputManager::IsNullptr(void)
{
	return instance_ == nullptr;
}

InputManager::InputManager(void) :
	joyDInState_(DINPUT_JOYSTATE()),
	joyXInState_(XINPUT_STATE()),
	infoEmpty_(Info()),
	mouseInfoEmpty_(MouseInfo()),
	mousePos_(Position2D()),
	mouseInput_(EXCEPTION_VALUE)
{
	for (int ii = 0; ii < PAD_MAX; ii++)padInfos_[ii] = JOYPAD_IN_STATE();
}

void InputManager::InitSimpleInput(void)
{
	SimpleInput::CreateInstance();

	static auto padNo = JOYPAD_NO::PAD1;
	static auto& simpleInput = SimpleInput::GetInstance();

	simpleInput.Add("moveForward", [&]() { return IsNew(KEY_INPUT_W) ||
		IsPadBtnPovNewTop(padNo) ||
		GetJPadInputState(padNo).aKeyLY < DECISION_VALUE; });

	simpleInput.Add("moveLeft", [&]() { return IsNew(KEY_INPUT_A) ||
		IsPadBtnPovNewLeft(padNo) ||
		GetJPadInputState(padNo).aKeyLX < DECISION_VALUE; });

	simpleInput.Add("moveBack", [&]() { return IsNew(KEY_INPUT_S) ||
		IsPadBtnPovNewDown(padNo) ||
		GetJPadInputState(padNo).aKeyLY > DECISION_VALUE; });

	simpleInput.Add("moveRight", [&]() { return IsNew(KEY_INPUT_D) ||
		IsPadBtnPovNewRight(padNo) ||
		GetJPadInputState(padNo).aKeyLX > DECISION_VALUE; });

	simpleInput.Add("normalAttack", [&]() { return IsTrgUpMouseLeft() ||
		IsPadBtnTrgUp(padNo, JOYPAD_BTN::LEFT); });

	simpleInput.Add("charge", [&]() { return IsClickMouseLeft() ||
		IsPadBtnNew(padNo, JOYPAD_BTN::LEFT); });

	simpleInput.Add("jump", [&]() { return IsTrgDown(KEY_INPUT_SPACE) ||
		IsPadBtnTrgDown(padNo, JOYPAD_BTN::DOWN); });

	simpleInput.Add("avoid", [&]() { return IsTrgUpMouseRight() ||
		IsPadBtnTrgDown(padNo, JOYPAD_BTN::RIGHT); });

	simpleInput.Add("skill", [&]() { return IsTrgDown(KEY_INPUT_Q) ||
		IsPadBtnTrgDown(padNo, JOYPAD_BTN::L); });

	simpleInput.Add("technique", [&]() { return IsTrgDown(KEY_INPUT_R) ||
		IsPadBtnTrgDown(padNo, JOYPAD_BTN::R); });

	simpleInput.Add("decision", [&]() { return IsTrgDownMouseLeft() ||
		IsTrgDown(KEY_INPUT_RETURN) ||
		(IsPadBtnTrgDown(padNo, JOYPAD_BTN::DOWN));});
	
	simpleInput.Add("shutDown", [&]() { return IsTrgDownMouseLeft(); });

	simpleInput.Add("shutDownContoroller", [&]() { return 
		IsPadBtnNew(padNo, JOYPAD_BTN::L_TRIGGER) &&
		IsPadBtnNew(padNo, JOYPAD_BTN::R_TRIGGER);});

	simpleInput.Add("pause", [&]() { return IsTrgDown(KEY_INPUT_ESCAPE) ||
		IsPadBtnTrgDown(padNo, JOYPAD_BTN::MENYU_TRIGGER); });

	simpleInput.Add("anyButton", [&]() { return (IsAllKey() || IsAllPadBtnInput()) && 
		!(IsPadBtnNew(padNo, JOYPAD_BTN::L_TRIGGER) ||
		  IsPadBtnNew(padNo, JOYPAD_BTN::R_TRIGGER)); });

	simpleInput.Add("selectLeft", [&]() { return IsTrgDown(KEY_INPUT_A) || 
		IsPadBtnPovTrgDown(padNo, JOYPAD_BTN::L_POV_LEFT); });

	simpleInput.Add("selectRight", [&]() { return IsTrgDown(KEY_INPUT_D) ||
		IsPadBtnPovTrgDown(padNo, JOYPAD_BTN::L_POV_RIGHT); });

	simpleInput.Add("selectUp", [&]() { return IsTrgDown(KEY_INPUT_W) ||
		IsPadBtnPovTrgDown(padNo, JOYPAD_BTN::L_POV_TOP); });

	simpleInput.Add("selectDown", [&]() { return IsTrgDown(KEY_INPUT_S) ||
		IsPadBtnPovTrgDown(padNo, JOYPAD_BTN::L_POV_DOWN); });

}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = keyInfos_.find(key);

	assert(it != keyInfos_.end() || !"InputManager::登録していないキーです");

	return it->second;
}

const InputManager::MouseInfo& InputManager::FindMouse(int key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

InputManager::JOYPAD_TYPE InputManager::GetJPadType(const JOYPAD_NO& no)const
{
	return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no)));
}

DINPUT_JOYSTATE InputManager::GetJPadDInputState(const JOYPAD_NO& no)
{
	// コントローラ情報
	GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
	return joyDInState_;
}

void InputManager::SetJPadInState(const JOYPAD_NO& jpNo)
{
	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(JOYPAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{

		stateNow.buttonsOld[i] = stateNow.buttonsNew[i];
		stateNow.buttonsNew[i] = stateNew.buttonsNew[i];

		stateNow.isOld[i] = stateNow.isNew[i];
		//stateNow.IsNew[i] = stateNow.ButtonsNew[i] == 128 || stateNow.ButtonsNew[i] == 255;
		stateNow.isNew[i] = stateNow.buttonsNew[i] > 0;

		stateNow.isTrgDown[i] = stateNow.isNew[i] && !stateNow.isOld[i];
		stateNow.isTrgUp[i] = !stateNow.isNew[i] && stateNow.isOld[i];
	}

	stateNow.isOldLa = stateNow.isNewLa;
	stateNow.isNewLa = (stateNow.aKeyLX != 0) || (stateNow.aKeyLY != 0);
	stateNow.isOldRa = stateNow.isNewRa;
	stateNow.isNewRa = (stateNow.aKeyRX != 0) || (stateNow.aKeyRY != 0);

	stateNow.isTrgDownLa = stateNow.isNewLa && !stateNow.isOldLa;
	stateNow.isTrgDownRa = stateNow.isNewRa && !stateNow.isOldRa;

	stateNow.aKeyLX = stateNew.aKeyLX;
	stateNow.aKeyLY = stateNew.aKeyLY;
	stateNow.aKeyRX = stateNew.aKeyRX;
	stateNow.aKeyRY = stateNew.aKeyRY;
}

XINPUT_STATE InputManager::GetJPadXInputState(const JOYPAD_NO& no)
{
	// コントローラ情報
	GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
	return joyXInState_;
}