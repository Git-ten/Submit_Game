#pragma once
#include <map>
#include<memory>
#include<DxLib.h>
#include"SimpleInput.h"
#include"../Utility/VectorHeader.h"

class InputManager
{

public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キー入力
	};

	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
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

	// ゲームコントローラーボタン
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

	// ゲームコントローラーの入力情報
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

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager& GetInstance(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	void Update(const SimpleInput::PL_NUMBER& number);

	// リソースの破棄
	void Destroy(void);

	// 判定を行うキーを追加
	void Add(int key);

	// 判定を行うキーをクリア
	void Clear(void);

	// キーの押下判定
	bool IsNew(int key) const;

	// キーの押下判定(押しっぱなしはNG)
	bool IsTrgDown(int key) const;

	// キーを離した時の判定
	bool IsTrgUp(int key) const;

	// マウス座標の取得
	Position2D GetMousePos(void) const;

	// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	int GetMouse(void) const;

	// マウスが左クリックされたか
	bool IsClickMouseLeft(void) const;

	// マウスが右クリックされたか
	bool IsClickMouseRight(void) const;

	// マウスが左クリックされたか(押しっぱなしはNG)
	bool IsTrgDownMouseLeft(void) const;
	//マウスが左クリックされた後離されたか
	bool IsTrgUpMouseLeft(void)const;


	// マウスが右クリックされたか(押しっぱなしはNG)
	bool IsTrgDownMouseRight(void) const;
	//マウスが左クリックされた後離されたか
	bool IsTrgUpMouseRight(void)const;

	// コントローラの入力情報を取得する
	JOYPAD_IN_STATE GetJPadInputState(const JOYPAD_NO& no);

	// ボタンが押された
	bool IsPadBtnNew(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	bool IsPadBtnTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	bool IsPadBtnTrgUp(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	bool IsPadBtnTrgDownLa(const JOYPAD_NO& no) const;
	bool IsPadBtnTrgDownRa(const JOYPAD_NO& no) const;

	//シンプルインプットの状態をの取得
	bool GetSimpleInput(const SimpleInput::SIMPLE& key)const;

	//シンプルインプットインスタンスの参照を取得
	SimpleInput& GetSimpleInputIns(void);

private:

	// キー情報
	struct Info
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// マウス
	struct MouseInfo
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	using KeyInfosData = std::map<int, InputManager::Info>;
	using MouseInfosData = std::map<int, InputManager::MouseInfo>;

	//パッドの最大数
	static constexpr int PAD_MAX = 5;

	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// シングルトン用インスタンス
	static InputManager* instance_;

	//シンプルインプットインスタンス
	std::unique_ptr<SimpleInput>simpleInput_;

	// キー情報
	KeyInfosData keyInfos_;
	Info infoEmpty_;

	// マウス情報
	MouseInfosData mouseInfos_;
	MouseInfo mouseInfoEmpty_;

	// マウスカーソルの位置
	Position2D mousePos_;
	
	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[PAD_MAX];

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputManager(void);
	// コピーコンストラクタも同様
	InputManager(const InputManager&);
	//デストラクタ
	~InputManager(void) = default;



	//内部でのみ使用する関数
	// 配列の中からキー情報を取得する
	const Info& Find(int key) const;
	// 配列の中からマウス情報を取得する
	const MouseInfo& FindMouse(int key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(const JOYPAD_NO& no)const;
	// コントローラの入力情報を取得する
	DINPUT_JOYSTATE GetJPadDInputState(const JOYPAD_NO& no);
	// コントローラの入力情報を更新する
	void SetJPadInState(const JOYPAD_NO& jpNo);

	// コントローラ(XBOX)の入力情報を取得する
	XINPUT_STATE GetJPadXInputState(const JOYPAD_NO& no);

};