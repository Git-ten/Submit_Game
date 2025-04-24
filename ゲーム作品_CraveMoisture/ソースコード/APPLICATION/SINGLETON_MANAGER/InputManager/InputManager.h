#pragma once
#include <map>
#include<memory>
#include<DxLib.h>
#include"../../../Utility/VectorHeader.h"

class InputManager
{

public:

	// �Q�[���R���g���[���[�̔F���ԍ�
	// DxLib�萔�ADX_INPUT_PAD1���ɑΉ�
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// �L�[���͂ƃp�b�h�P����
		PAD1,				// �p�b�h�P����
		PAD2,				// �p�b�h�Q����
		PAD3,				// �p�b�h�R����
		PAD4,				// �p�b�h�S����
		INPUT_KEY = 4096	// �L�[����
	};

	// �Q�[���R���g���[���[�^�C�v
	// DxLib�萔�ADX_OTHER���ɑΉ�
	enum class JOYPAD_TYPE
	{
		OTHER = 0,
		XBOX_360,					//XBOX360
		XBOX_ONE,					//XBOX_ONE
		DUAL_SHOCK_4,				//DUAL_SHOCK_4
		DUAL_SENSE,					//XBOXDUAL_SENSE360
		SWITCH_JOY_CON_L,			//SWITCH_JOY_CON_L
		SWITCH_JOY_CON_R,			//SWITCH_JOY_CON_R
		SWITCH_PRO_CTRL,			//PRO_CONTROLLER
		MAX
	};

	// �Q�[���R���g���[���[�{�^��
	enum class JOYPAD_BTN
	{
		LEFT = 0,		//���{�^��(X)
		RIGHT,			//�E�{�^��(B)
		TOP,			//��{�^��(Y)
		DOWN,			//���{�^��(A)
		R,				//RB
		L,				//LB
		R_TRIGGER,		//RT
		L_TRIGGER,		//LT
		MENYU_TRIGGER,	//Menyu
		R_STICK_BUTTON,	//�E�X�e�B�b�N�̉�������
		L_POV_LEFT,		// ���\���L�[�̏��
		L_POV_RIGHT,	// ���\���L�[�̏��
		L_POV_TOP,		// ���\���L�[�̏��
		L_POV_DOWN,		// ���\���L�[�̏��
		MAX
	};

	// �Q�[���R���g���[���[�̓��͏��
	struct JOYPAD_IN_STATE
	{
		unsigned char buttonsOld[static_cast<int>(JOYPAD_BTN::MAX)];	//1�t���[���O�̃{�^���̏��
		unsigned char buttonsNew[static_cast<int>(JOYPAD_BTN::MAX)];	//���݂̃{�^���̏��
		bool isOld[static_cast<int>(JOYPAD_BTN::MAX)];					//1�t���[���O�̃{�^���̏��(bool�^)
		bool isNew[static_cast<int>(JOYPAD_BTN::MAX)];					//���݂̃{�^���̏��(bool�^)
		bool isTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];				//�L�[�̉�������(����������)
		bool isTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];				//�L�[�̉�������(��������)
		int aKeyLX;														//���X�e�B�b�N��X���ω���
		int aKeyLY;														//���X�e�B�b�N��Y���ω���
		int aKeyRX;														//�E�X�e�B�b�N��X���ω���
		int aKeyRY;														//�E�X�e�B�b�N��Y���ω���
		bool isOldLa;													//1�t���[���O�̍��X�e�B�b�N�̏��
		bool isOldRa;													//1�t���[���O�̉E�X�e�B�b�N�̏��
		bool isNewLa;													//���݂̍��X�e�B�b�N�̏��
		bool isNewRa;													//���݂̉E�X�e�B�b�N�̏��
		bool isTrgDownLa;												//���X�e�B�b�N�̉������(����������)
		bool isTrgDownRa;												//�E�X�e�B�b�N�̉������(����������)
	};

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static InputManager& GetInstance(void);


	//������
	void Init(void);
	//�X�V
	void Update(void);
	//void Update(const SimpleInput::PL_NUMBER& number);
	// ���\�[�X�̔j��
	void Destroy(void);


	// ������s���L�[��ǉ�
	void Add(int key);


	// �L�[�̉�������
	const bool IsNew(int key) const;


	// �L�[�̉�������(�������ςȂ���NG)
	const bool IsTrgDown(int key) const;


	// �L�[�𗣂������̔���
	const bool IsTrgUp(int key) const;


	// �}�E�X���W�̎擾
	const Position2D& GetMousePos(void) const;


	//�]���̂��ƂłȂ��Ǝg��Ȃ�
	//�}�E�X���W���Z�b�g
	void SetMousePos(Position2D pos);


	// �}�E�X�̃N���b�N��Ԃ��擾(MOUSE_INPUT_LEFT�ARIGHT,MIDDLE)
	const int GetMouse(void) const;


	// �}�E�X�����N���b�N���ꂽ��
	const bool IsClickMouseLeft(void) const;


	// �}�E�X���E�N���b�N���ꂽ��
	const bool IsClickMouseRight(void) const;


	//�}�E�X�̒����{�^�����������ꂽ��
	const bool IsClickMouseMiddle(void)const;


	// �}�E�X�����N���b�N���ꂽ��(�������ςȂ���NG)
	const bool IsTrgDownMouseLeft(void) const;
	//�}�E�X�����N���b�N���ꂽ�㗣���ꂽ��
	const bool IsTrgUpMouseLeft(void)const;


	// �}�E�X���E�N���b�N���ꂽ��(�������ςȂ���NG)
	const bool IsTrgDownMouseRight(void) const;
	//�}�E�X�����N���b�N���ꂽ�㗣���ꂽ��
	const bool IsTrgUpMouseRight(void)const;
	

	// �}�E�X�������{�^�����������ꂽ��(�������ςȂ���NG)
	const bool IsTrgDownMouseMiddle(void) const;
	//�}�E�X�������{�^�����������ꂽ�㗣���ꂽ��
	const bool IsTrgUpMouseMiddle(void)const;


	// �R���g���[���̓��͏����擾����
	JOYPAD_IN_STATE GetJPadInputState(const JOYPAD_NO& no);


	// �S�ẴL�[�̓��͏��(1�ł����͂���Ă���Ȃ�true��Ԃ�)
	const bool IsAllKey(void);
	const bool IsAllPadBtnInput(void);

	// �R���g���[���̐ڑ������擾
	int GetJPadNum(void);

	// �{�^���������ꂽ
	const bool IsPadBtnNew(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	const bool IsPadBtnTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	const bool IsPadBtnTrgUp(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	const bool IsPadBtnTrgDownLa(const JOYPAD_NO& no) const;
	const bool IsPadBtnTrgDownRa(const JOYPAD_NO& no) const;

	// �\���L�[�̓��͂��擾
	const bool IsPadBtnPovNewTop(const JOYPAD_NO& no);
	const bool IsPadBtnPovTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn);
	const bool IsPadBtnPovNewLeft(const JOYPAD_NO& no);
	const bool IsPadBtnPovNewDown(const JOYPAD_NO& no);
	const bool IsPadBtnPovNewRight(const JOYPAD_NO& no);

	// NULL�ł��邩
	static const bool IsNullptr(void);

private:

	//�p�b�h�̍ő吔
	static constexpr int PAD_MAX = 5;

	// �L�[���
	struct Info
	{
		int key;			// �L�[ID
		bool keyOld;		// 1�t���[���O�̉������
		bool keyNew;		// ���t���[���̉������
		bool keyTrgDown;	// ���t���[���Ń{�^���������ꂽ��
		bool keyTrgUp;		// ���t���[���Ń{�^���������ꂽ��
	};

	// �}�E�X
	struct MouseInfo
	{
		int key;			// �L�[ID
		bool keyOld;		// 1�t���[���O�̉������
		bool keyNew;		// ���t���[���̉������
		bool keyTrgDown;	// ���t���[���Ń{�^���������ꂽ��
		bool keyTrgUp;		// ���t���[���Ń{�^���������ꂽ��
	};

	//�L�[�̎�ނ��Ƃ̏��
	using KeyInfosData = std::map<int, InputManager::Info>;
	//�}�E�X�̏��
	using MouseInfosData = std::map<int, InputManager::MouseInfo>;


	// �V���O���g���p�C���X�^���X
	static InputManager* instance_;


	// �R���g���[�����
	DINPUT_JOYSTATE joyDInState_;


	// �R���g���[�����(XBOX)
	XINPUT_STATE joyXInState_;


	// �L�[���
	KeyInfosData keyInfos_;
	Info infoEmpty_;


	// �}�E�X���
	MouseInfosData mouseInfos_;
	MouseInfo mouseInfoEmpty_;


	// �}�E�X�J�[�\���̈ʒu
	Position2D mousePos_;
	

	// �}�E�X�{�^���̓��͏��
	int mouseInput_;


	// �p�b�h���
	JOYPAD_IN_STATE padInfos_[PAD_MAX];



	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	InputManager(void);
	// �R�s�[�R���X�g���N�^�����l
	InputManager(const InputManager&);
	//�f�X�g���N�^
	~InputManager(void) = default;



	//�����ł̂ݎg�p����֐�
	// �V���v���C���v�b�g��������
	void InitSimpleInput(void);
	
	// �z��̒�����L�[�����擾����
	const Info& Find(int key) const;
	// �z��̒�����}�E�X�����擾����
	const MouseInfo& FindMouse(int key) const;


	// �ڑ����ꂽ�R���g���[���̎�ʂ��擾����
	JOYPAD_TYPE GetJPadType(const JOYPAD_NO& no)const;
	// �R���g���[���̓��͏����擾����
	DINPUT_JOYSTATE GetJPadDInputState(const JOYPAD_NO& no);
	// �R���g���[���̓��͏����X�V����
	void SetJPadInState(const JOYPAD_NO& jpNo);


	// �R���g���[��(XBOX)�̓��͏����擾����
	XINPUT_STATE GetJPadXInputState(const JOYPAD_NO& no);

};