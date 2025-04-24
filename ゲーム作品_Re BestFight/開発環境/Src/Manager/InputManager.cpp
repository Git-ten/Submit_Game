#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputManager::CreateInstance();
	}
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
	InputManager::GetInstance().Add(KEY_INPUT_J);
	InputManager::GetInstance().Add(KEY_INPUT_I);
	InputManager::GetInstance().Add(KEY_INPUT_L);
	InputManager::GetInstance().Add(KEY_INPUT_SPACE);
	InputManager::GetInstance().Add(KEY_INPUT_Q);
	InputManager::GetInstance().Add(KEY_INPUT_LSHIFT);
	InputManager::GetInstance().Add(KEY_INPUT_G);
	InputManager::GetInstance().Add(KEY_INPUT_E);

	InputManager::GetInstance().Add(KEY_INPUT_UP);
	InputManager::GetInstance().Add(KEY_INPUT_DOWN);
	InputManager::GetInstance().Add(KEY_INPUT_LEFT);
	InputManager::GetInstance().Add(KEY_INPUT_RIGHT);

	InputManager::GetInstance().Add(KEY_INPUT_RETURN);
	InputManager::GetInstance().Add(KEY_INPUT_Z);

	InputManager::GetInstance().Add(KEY_INPUT_ESCAPE);

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

	SetUseJoypadVibrationFlag(true);

	//シンプルインプット
	simpleInput_ = std::make_unique<SimpleInput>();
	simpleInput_->Init();
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

	simpleInput_->Update();
}

void InputManager::Update(const SimpleInput::PL_NUMBER& number)
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

	simpleInput_->Update(number);
}

void InputManager::Destroy(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
	delete instance_;
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

void InputManager::Clear(void)
{
	keyInfos_.clear();
}

bool InputManager::IsNew(int key) const
{
	return Find(key).keyNew;
}

bool InputManager::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

bool InputManager::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

Position2D InputManager::GetMousePos(void) const
{
	return mousePos_;
}

int InputManager::GetMouse(void) const
{
	return mouseInput_;
}

bool InputManager::IsClickMouseLeft(void) const
{
	return mouseInput_ == MOUSE_INPUT_LEFT;
}

bool InputManager::IsClickMouseRight(void) const
{
	return mouseInput_ == MOUSE_INPUT_RIGHT;
}

bool InputManager::IsTrgDownMouseLeft(void) const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown;
}

bool InputManager::IsTrgUpMouseLeft(void)const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgUp;
}

bool InputManager::IsTrgDownMouseRight(void) const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown;
}

bool InputManager::IsTrgUpMouseRight(void)const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgUp;
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

bool InputManager::IsPadBtnNew(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const
{
	return padInfos_[static_cast<int>(no)].isNew[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const
{
	return padInfos_[static_cast<int>(no)].isTrgDown[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgUp(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const
{
	return padInfos_[static_cast<int>(no)].isTrgUp[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgDownLa(const JOYPAD_NO& no) const
{
	return padInfos_[static_cast<int>(no)].isTrgDownLa;
}

bool InputManager::IsPadBtnTrgDownRa(const JOYPAD_NO& no) const
{
	return padInfos_[static_cast<int>(no)].isTrgDownRa;
}

bool InputManager::GetSimpleInput(const SimpleInput::SIMPLE& key)const
{
	return simpleInput_->GetSimpleKey(key);
}

SimpleInput& InputManager::GetSimpleInputIns(void)
{
	return *simpleInput_;
}

InputManager::InputManager(void)
{
	mouseInput_ = -1;
	mousePos_ = Position2D();
	infoEmpty_ = Info();
	joyDInState_ = DINPUT_JOYSTATE();
	joyXInState_ = XINPUT_STATE();
	mouseInfoEmpty_ = MouseInfo();
	for (int ii = 0; ii < PAD_MAX; ii++)padInfos_[ii] = JOYPAD_IN_STATE();
}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = keyInfos_.find(key);
	if (it != keyInfos_.end())
	{
		return it->second;
	}

	return infoEmpty_;

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