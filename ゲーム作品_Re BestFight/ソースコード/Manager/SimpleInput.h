#pragma once

//InputManagerの拡張
class InputManager;

class SimpleInput
{
public:

	//キーID
	enum class SIMPLE
	{
		KEY_WEEKATC,//弱攻撃
		KEY_STRONGATC,//強攻撃
		KEY_JUMP,//ジャンプ
		KEY_DEFENCE,//防御
		KEY_QUICKMOVE,//高速移動
		KEY_AVOID,//回避
		KEY_SKILL_1,//必殺1-2
		KEY_SKILL_2,//必殺2-2
		KEY_UP,//移動:上
		KEY_DOWN,//移動:下
		KEY_LEFT,//移動:左
		KEY_RIGHT,//移動:右
		MAX
	};


	//入力するプレイヤー
	enum class PL_NUMBER
	{
		PL_1,//プレイヤー1
		PL_2,//プレイヤー2
	};


	//コンストラクタ
	SimpleInput(void);

	//デストラクタ
	~SimpleInput(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	void Update(const PL_NUMBER& number);

	// リソースの破棄
	void Destroy(void);

	//キーの状態を取得
	bool GetSimpleKey(const SIMPLE& key)const;

private:

	//登録しているキーの更新
	bool stateKey_[static_cast<int>(SIMPLE::MAX)];
	
};