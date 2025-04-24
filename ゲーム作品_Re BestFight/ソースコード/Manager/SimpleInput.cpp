#include"InputManager.h"
#include "SimpleInput.h"
#include<DxLib.h>

SimpleInput::SimpleInput()
{
	for (int ii = 0; ii < static_cast<int>(SIMPLE::MAX); ii++)stateKey_[ii] = false;
}

SimpleInput::~SimpleInput()
{

}

void SimpleInput::Init(void)
{
	int max = static_cast<int>(SIMPLE::MAX);

	for (int ii = 0; ii < max; ii++)
	{
		stateKey_[ii] = false;
	}
}

void SimpleInput::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	auto controll = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);


	//�V���v���C���v�b�g����
	//���蓮�œo�^����
	//�V���v���C���v�b�g�͏�ɍX�V����邽�߁AUpdate�ŃL�[��o�^���Ă���
	//------------------------------------------------
	//��U��
	int idx = static_cast<int>(SIMPLE::KEY_WEEKATC);
	stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_J) ||
					   ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT)) ? true : false;

	//���U��
	idx = static_cast<int>(SIMPLE::KEY_STRONGATC);
	stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_I) ||
					  ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP)) ? true : false;

	//�W�����v
	idx = static_cast<int>(SIMPLE::KEY_JUMP);
	stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_SPACE) ||
					  ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)) ? true : false;

	//�h��
	idx = static_cast<int>(SIMPLE::KEY_DEFENCE);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_Q) ||
					  ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L)) ? true : false;
	
	//�����ړ�
	idx = static_cast<int>(SIMPLE::KEY_QUICKMOVE);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_E) ||
					  ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R)) ? true : false;

	//���
	idx = static_cast<int>(SIMPLE::KEY_AVOID);
	stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_L) ||
					  ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) ? true : false;

	//�K�E1
	idx = static_cast<int>(SIMPLE::KEY_SKILL_1);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_LSHIFT) ||
					  ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER)) ? true : false;

	//�K�E2
	idx = static_cast<int>(SIMPLE::KEY_SKILL_2);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_G) ||
					  ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER)) ? true : false;

	//�ړ�:��
	idx = static_cast<int>(SIMPLE::KEY_UP);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_W) ||
					 (0 > controll.aKeyLY)) ? true : false;

	//�ړ�:��
	idx = static_cast<int>(SIMPLE::KEY_DOWN);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_S) ||
					 (0 < controll.aKeyLY)) ? true : false;

	//�ړ�:��
	idx = static_cast<int>(SIMPLE::KEY_LEFT);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_A) ||
					 (0 > controll.aKeyLX)) ? true : false;

	//�ړ�:�E
	idx = static_cast<int>(SIMPLE::KEY_RIGHT);
	stateKey_[idx] = (ins.IsNew(KEY_INPUT_D) ||
					 (0 < controll.aKeyLX)) ? true : false;
	//------------------------------------------------
}

void SimpleInput::Update(const PL_NUMBER& number)
{
	InputManager& ins = InputManager::GetInstance();
	auto controll_1 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	auto controll_2 = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2);


	//�V���v���C���v�b�g����
	//���蓮�œo�^����
	//�V���v���C���v�b�g�͏�ɍX�V����邽�߁AUpdate�ŃL�[��o�^���Ă���
	//------------------------------------------------
	if (number == PL_NUMBER::PL_1)
	{
		//��U��
		int idx = static_cast<int>(SIMPLE::KEY_WEEKATC);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_J) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT)) ? true : false;

		//���U��
		idx = static_cast<int>(SIMPLE::KEY_STRONGATC);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_I) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP)) ? true : false;

		//�W�����v
		idx = static_cast<int>(SIMPLE::KEY_JUMP);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_SPACE) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)) ? true : false;

		//�h��
		idx = static_cast<int>(SIMPLE::KEY_DEFENCE);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_Q) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L)) ? true : false;

		//�����ړ�
		idx = static_cast<int>(SIMPLE::KEY_QUICKMOVE);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_E) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R)) ? true : false;

		//���
		idx = static_cast<int>(SIMPLE::KEY_AVOID);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_L) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) ? true : false;

		//�K�E1-2
		idx = static_cast<int>(SIMPLE::KEY_SKILL_1);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_LSHIFT) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER)) ? true : false;

		//�K�E2-2
		idx = static_cast<int>(SIMPLE::KEY_SKILL_2);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_G) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER)) ? true : false;

		//�ړ�:��
		idx = static_cast<int>(SIMPLE::KEY_UP);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_W) ||
			(0 > controll_1.aKeyLY)) ? true : false;

		//�ړ�:��
		idx = static_cast<int>(SIMPLE::KEY_DOWN);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_S) ||
			(0 < controll_1.aKeyLY)) ? true : false;

		//�ړ�:��
		idx = static_cast<int>(SIMPLE::KEY_LEFT);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_A) ||
			(0 > controll_1.aKeyLX)) ? true : false;

		//�ړ�:�E
		idx = static_cast<int>(SIMPLE::KEY_RIGHT);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_D) ||
			(0 < controll_1.aKeyLX)) ? true : false;
	}
	else
	{
		//��U��
		int idx = static_cast<int>(SIMPLE::KEY_WEEKATC);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_J) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::LEFT)) ? true : false;

		//���U��
		idx = static_cast<int>(SIMPLE::KEY_STRONGATC);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_I) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::TOP)) ? true : false;

		//�W�����v
		idx = static_cast<int>(SIMPLE::KEY_JUMP);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_SPACE) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::DOWN)) ? true : false;

		//�h��
		idx = static_cast<int>(SIMPLE::KEY_DEFENCE);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_Q) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::L)) ? true : false;

		//�����ړ�
		idx = static_cast<int>(SIMPLE::KEY_QUICKMOVE);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_E) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R)) ? true : false;

		//���
		idx = static_cast<int>(SIMPLE::KEY_AVOID);
		stateKey_[idx] = (ins.IsTrgDown(KEY_INPUT_L) ||
			ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT)) ? true : false;

		//�K�E1-2
		idx = static_cast<int>(SIMPLE::KEY_SKILL_1);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_LSHIFT) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::L_TRIGGER)) ? true : false;

		//�K�E2-2
		idx = static_cast<int>(SIMPLE::KEY_SKILL_2);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_G) ||
			ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER)) ? true : false;

		//�ړ�:��
		idx = static_cast<int>(SIMPLE::KEY_UP);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_W) ||
			(0 > controll_2.aKeyLY)) ? true : false;

		//�ړ�:��
		idx = static_cast<int>(SIMPLE::KEY_DOWN);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_S) ||
			(0 < controll_2.aKeyLY)) ? true : false;

		//�ړ�:��
		idx = static_cast<int>(SIMPLE::KEY_LEFT);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_A) ||
			(0 > controll_2.aKeyLX)) ? true : false;

		//�ړ�:�E
		idx = static_cast<int>(SIMPLE::KEY_RIGHT);
		stateKey_[idx] = (ins.IsNew(KEY_INPUT_D) ||
			(0 < controll_2.aKeyLX)) ? true : false;
	}
	//------------------------------------------------
}

void SimpleInput::Destroy(void)
{

}

bool SimpleInput::GetSimpleKey(const SIMPLE& key)const
{
	int idx = std::move(static_cast<int>(key));
	return stateKey_[idx];
}