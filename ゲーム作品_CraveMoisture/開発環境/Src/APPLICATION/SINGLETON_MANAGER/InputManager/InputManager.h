#pragma once
#include <map>
#include<memory>
#include<DxLib.h>
#include"../../../Utility/VectorHeader.h"

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
		XBOX_360,					//XBOX360
		XBOX_ONE,					//XBOX_ONE
		DUAL_SHOCK_4,				//DUAL_SHOCK_4
		DUAL_SENSE,					//XBOXDUAL_SENSE360
		SWITCH_JOY_CON_L,			//SWITCH_JOY_CON_L
		SWITCH_JOY_CON_R,			//SWITCH_JOY_CON_R
		SWITCH_PRO_CTRL,			//PRO_CONTROLLER
		MAX
	};

	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		LEFT = 0,		//左ボタン(X)
		RIGHT,			//右ボタン(B)
		TOP,			//上ボタン(Y)
		DOWN,			//下ボタン(A)
		R,				//RB
		L,				//LB
		R_TRIGGER,		//RT
		L_TRIGGER,		//LT
		MENYU_TRIGGER,	//Menyu
		R_STICK_BUTTON,	//右スティックの押し込み
		L_POV_LEFT,		// 左十字キーの状態
		L_POV_RIGHT,	// 左十字キーの状態
		L_POV_TOP,		// 左十字キーの状態
		L_POV_DOWN,		// 左十字キーの状態
		MAX
	};

	// ゲームコントローラーの入力情報
	struct JOYPAD_IN_STATE
	{
		unsigned char buttonsOld[static_cast<int>(JOYPAD_BTN::MAX)];	//1フレーム前のボタンの状態
		unsigned char buttonsNew[static_cast<int>(JOYPAD_BTN::MAX)];	//現在のボタンの状態
		bool isOld[static_cast<int>(JOYPAD_BTN::MAX)];					//1フレーム前のボタンの状態(bool型)
		bool isNew[static_cast<int>(JOYPAD_BTN::MAX)];					//現在のボタンの状態(bool型)
		bool isTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];				//キーの押下判定(押下した時)
		bool isTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];				//キーの押下判定(離した時)
		int aKeyLX;														//左スティックのX軸変化量
		int aKeyLY;														//左スティックのY軸変化量
		int aKeyRX;														//右スティックのX軸変化量
		int aKeyRY;														//右スティックのY軸変化量
		bool isOldLa;													//1フレーム前の左スティックの状態
		bool isOldRa;													//1フレーム前の右スティックの状態
		bool isNewLa;													//現在の左スティックの状態
		bool isNewRa;													//現在の右スティックの状態
		bool isTrgDownLa;												//左スティックの押下状態(押下した時)
		bool isTrgDownRa;												//右スティックの押下状態(押下した時)
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager& GetInstance(void);


	//初期化
	void Init(void);
	//更新
	void Update(void);
	//void Update(const SimpleInput::PL_NUMBER& number);
	// リソースの破棄
	void Destroy(void);


	// 判定を行うキーを追加
	void Add(int key);


	// キーの押下判定
	const bool IsNew(int key) const;


	// キーの押下判定(押しっぱなしはNG)
	const bool IsTrgDown(int key) const;


	// キーを離した時の判定
	const bool IsTrgUp(int key) const;


	// マウス座標の取得
	const Position2D& GetMousePos(void) const;


	//余程のことでないと使わない
	//マウス座標をセット
	void SetMousePos(Position2D pos);


	// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT,MIDDLE)
	const int GetMouse(void) const;


	// マウスが左クリックされたか
	const bool IsClickMouseLeft(void) const;


	// マウスが右クリックされたか
	const bool IsClickMouseRight(void) const;


	//マウスの中央ボタンが押下されたか
	const bool IsClickMouseMiddle(void)const;


	// マウスが左クリックされたか(押しっぱなしはNG)
	const bool IsTrgDownMouseLeft(void) const;
	//マウスが左クリックされた後離されたか
	const bool IsTrgUpMouseLeft(void)const;


	// マウスが右クリックされたか(押しっぱなしはNG)
	const bool IsTrgDownMouseRight(void) const;
	//マウスが左クリックされた後離されたか
	const bool IsTrgUpMouseRight(void)const;
	

	// マウスが中央ボタンが押下されたか(押しっぱなしはNG)
	const bool IsTrgDownMouseMiddle(void) const;
	//マウスが中央ボタンが押下された後離されたか
	const bool IsTrgUpMouseMiddle(void)const;


	// コントローラの入力情報を取得する
	JOYPAD_IN_STATE GetJPadInputState(const JOYPAD_NO& no);


	// 全てのキーの入力状態(1つでも入力されているならtrueを返す)
	const bool IsAllKey(void);
	const bool IsAllPadBtnInput(void);

	// コントローラの接続数を取得
	int GetJPadNum(void);

	// ボタンが押された
	const bool IsPadBtnNew(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	const bool IsPadBtnTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	const bool IsPadBtnTrgUp(const JOYPAD_NO& no, const JOYPAD_BTN& btn) const;
	const bool IsPadBtnTrgDownLa(const JOYPAD_NO& no) const;
	const bool IsPadBtnTrgDownRa(const JOYPAD_NO& no) const;

	// 十字キーの入力を取得
	const bool IsPadBtnPovNewTop(const JOYPAD_NO& no);
	const bool IsPadBtnPovTrgDown(const JOYPAD_NO& no, const JOYPAD_BTN& btn);
	const bool IsPadBtnPovNewLeft(const JOYPAD_NO& no);
	const bool IsPadBtnPovNewDown(const JOYPAD_NO& no);
	const bool IsPadBtnPovNewRight(const JOYPAD_NO& no);

	// NULLであるか
	static const bool IsNullptr(void);

private:

	//パッドの最大数
	static constexpr int PAD_MAX = 5;

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

	//キーの種類ごとの情報
	using KeyInfosData = std::map<int, InputManager::Info>;
	//マウスの情報
	using MouseInfosData = std::map<int, InputManager::MouseInfo>;


	// シングルトン用インスタンス
	static InputManager* instance_;


	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;


	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;


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
	// シンプルインプットを初期化
	void InitSimpleInput(void);
	
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