#pragma once
#include <map>
#include<memory>
#include<DxLib.h>
#include"SimpleInput.h"
#include"../Utility/VectorHeader.h"

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
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		SWITCH_JOY_CON_L,
		SWITCH_JOY_CON_R,
		SWITCH_PRO_CTRL,
		MAX
	};

	// �Q�[���R���g���[���[�{�^��
	enum class JOYPAD_BTN
	{
		LEFT = 0,
		RIGHT,
		TOP,
		DOWN,
		R,
		L,
		R_TRIGGER,
		L_TRIGGER,
		MENYU_TRIGGER,
		MAX
	};

	// �Q�[���R���g���[���[�̓��͏��
	struct JOYPAD_IN_STATE
	{
		unsigned char buttonsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		unsigned char buttonsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool isOld[static_cast<int>(JOYPAD_BTN::MAX)];
		bool isNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool isTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];
		bool isTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];
		int aKeyLX;
		int aKeyLY;
		int aKeyRX;
		int aKeyRY;
		bool isOldLa;
		bool isOldRa;
		bool isNewLa;
		bool isNewRa;
		bool isTrgDownLa;
		bool isTrgDownRa;
	};

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static InputManager& GetInstance(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	void Update(const SimpleInput::PL_NUMBER& number);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ������s���L�[��ǉ�
	void Add(int key);

	// ������s���L�[���N���A
	void Clear(void);

	// �L�[�̉�������
	bool IsNew(int key) const;

	// �L�[�̉�������(�������ςȂ���NG)
	bool IsTrgDown(int key) const;

	// �L�[�𗣂������̔���
	bool IsTrgUp(int key) const;

	// �}�E�X���W�̎擾
	Position2D GetMousePos(void) const;

	// �}�E�X�̃N���b�N��Ԃ��擾(MOUSE_INPUT_LEFT�ARIGHT)
	int GetMouse(void) const;

	// �}�E�X�����N���b�N���ꂽ��
	bool IsClickMouseLeft(void) const;

	// �}�E�X���E�N���b�N���ꂽ��
	bool IsClickMouseRight(void) const;

	// �}�E�X�����N���b�N���ꂽ��(�������ςȂ���NG)
	bool IsTrgDownMouseLeft(void) const;
	//�}�E�X�����N���b�N���ꂽ�㗣���ꂽ��
	bool IsTrgUpMouseLeft(void)const;


	// �}�E�X���E�N���b�N���ꂽ��(�������ςȂ���NG)
	bool IsTrgDownMouseRight(void) const;
	//�}�E�X�����N���b�N���ꂽ�㗣���ꂽ��
	bool IsTrgUpMouseRight(void)const;

	// �R���g���[���̓��͏����擾����
	JOYPAD_IN_STATE GetJPadInputState(const JOYPAD_NO& no);

	// �{�^���������ꂽ
	bool IsPadBtnNew(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	bool IsPadBtnTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	bool IsPadBtnTrgUp(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	bool IsPadBtnTrgDownLa(const JOYPAD_NO& no) const;
	bool IsPadBtnTrgDownRa(const JOYPAD_NO& no) const;

	//�V���v���C���v�b�g�̏�Ԃ��̎擾
	bool GetSimpleInput(const SimpleInput::SIMPLE& key)const;

	//�V���v���C���v�b�g�C���X�^���X�̎Q�Ƃ��擾
	SimpleInput& GetSimpleInputIns(void);

private:

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

	using KeyInfosData = std::map<int, InputManager::Info>;
	using MouseInfosData = std::map<int, InputManager::MouseInfo>;

	//�p�b�h�̍ő吔
	static constexpr int PAD_MAX = 5;

	// �R���g���[�����
	DINPUT_JOYSTATE joyDInState_;

	// �R���g���[�����(XBOX)
	XINPUT_STATE joyXInState_;

	// �V���O���g���p�C���X�^���X
	static InputManager* instance_;

	//�V���v���C���v�b�g�C���X�^���X
	std::unique_ptr<SimpleInput>simpleInput_;

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