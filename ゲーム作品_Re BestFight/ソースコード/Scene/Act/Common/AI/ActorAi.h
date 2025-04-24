#pragma once
#include<memory>
#include<map>
#include "BehaviorBase.h"
#include"../../../../Utility/VectorHeader.h"

class ActorBase;

class ActorAi : public BehaviorBase
{
public:

	//AIモード
	enum class MODE
	{
		AUTO,//CPU操作
		MANUAL,//手動
	};

	//時間
	enum class TIME
	{
		FRAME_TIME,//フレームタイム
		ATTACK_RAG,//攻撃のラグ
		QUICK_TIME,//高速移動時間
		QUICK_RAG,//高速移動のラグ
		AVOID_TIME,//回避時間
		JUST_RECEP_TIME,//ジャスト判定時間
		JUST_CHANCE_TIME,//ジャストカウンターチャンス時間
		GURD_RAG,//ガードブレイクされた後再びガードできるまでの時間

		//自動モード専用時間
		COUNTER_RAG,//カウンター攻撃をするまでのラグ
	};

	//インデックス名
	enum INDEX
	{
		//移動条件
		//自動
		INDEX_CON_AUTO_MOVE_FAR = 1,
		INDEX_CON_AUTO_MOVE_MEDIUM,

		//逃げる条件
		//自動
		INDEX_CON_AUTO_DEFENCE,
		INDEX_CON_AUTO_AVOID,
		INDEX_CON_AUTO_RUN,

		//攻撃条件
		//自動
		INDEX_CON_AUTO_WEEK,
		INDEX_CON_AUTO_STRONG,

		//移動アクション
		//自動
		INDEX_ACT_AUTO_MOVE_FAR,
		INDEX_ACT_AUTO_MOVE_MEDIUM,

		//逃げるアクション
		//自動
		INDEX_ACT_AUTO_DEFENCE,
		INDEX_ACT_AUTO_AVOID,
		INDEX_ACT_AUTO_RUN,

		//カウンター攻撃アクション
		//自動
		INDEX_ACT_AUTO_COUNTER,

		//移動条件
		//手動
		INDEX_CON_MOVE_UP,
		INDEX_CON_MOVE_DOWN,
		INDEX_CON_MOVE_LEFT,
		INDEX_CON_MOVE_RIGHT,
		INDEX_CON_MOVE_LEFTUP,
		INDEX_CON_MOVE_RIGHTUP,
		INDEX_CON_MOVE_LEFTDOWN,
		INDEX_CON_MOVE_RIGHTDOWN,

		//手動
		INDEX_ACT_MOVE_UP,
		INDEX_ACT_MOVE_DOWN,
		INDEX_ACT_MOVE_LEFT,
		INDEX_ACT_MOVE_RIGHT,
		INDEX_ACT_MOVE_LEFTUP,
		INDEX_ACT_MOVE_RIGHTUP,
		INDEX_ACT_MOVE_LEFTDOWN,
		INDEX_ACT_MOVE_RIGHTDOWN,

		//コマンド入力条件
		INDEX_CON_COMAND_WEEK,
		INDEX_CON_COMAND_WEEKBRANCH,
		INDEX_CON_COMAND_STRONG,
		INDEX_CON_COMAND_STRONGBRANCH,
		INDEX_CON_COMAND_AVOID,
		INDEX_CON_COMAND_QUICKMOVE,
		INDEX_CON_COMAND_JUMP,
		INDEX_CON_COMAND_FALLATTACK,
		INDEX_CON_COMAND_DEFENCE,
		INDEX_CON_COMAND_UNLEASHSKILL,
		INDEX_CON_COMAND_COUNTERATTACK,

		//コマンドアクション
		INDEX_ACT_COMAND_WEEK_1,
		INDEX_ACT_COMAND_WEEK_2,
		INDEX_ACT_COMAND_WEEK_3,
		INDEX_ACT_COMAND_WEEK_4,
		INDEX_ACT_COMAND_WEEK_5,
		INDEX_ACT_COMAND_WEEK_6,
		INDEX_ACT_COMAND_STRONG_1,
		INDEX_ACT_COMAND_STRONG_2,
		INDEX_ACT_COMAND_STRONG_3,
		INDEX_ACT_COMAND_AVOID,
		INDEX_ACT_COMAND_QUICKMOVE,
		INDEX_ACT_COMAND_JUMP,
		INDEX_ACT_COMAND_FALLATTACK,
		INDEX_ACT_COMAND_DEFENCE,
		INDEX_ACT_COMAND_UNLEASHSKILL,
		INDEX_ACT_COMAND_COUNTERATTACK,
		INDEX_ACT_COMAND_FAIL_COUNTERATTACK,
	};

	//相手との距離基準
	//遠距離
	static constexpr float DISTANCE_FAR = 1500.0f;
	//中距離
	static constexpr float DISTANCE_MEDIUM = 100.0f;

	//デフォルトのフレーム時間
	static constexpr float DEFAULT_FRAME_TIME = 0.01f;
	//デフォルトのフレーム速度
	static constexpr float DEFAULT_FRAME_SPEED = 1.0f;

	//コンストラクタ
	ActorAi(std::weak_ptr<ActorBase> actor);

	//デストラクタ
	virtual ~ActorAi(void)override;

	//更新
	void Update(void)override;

	//時間系
	void SetTime(const TIME& type, const float t);
	void SetFrameSpeed(float speed);
	void SetFrameDefaultSpeed(void);

	//ダメージフラグを取得
	bool GetDamageFlag(void)const;
	//ダメージフラグをセット
	void SetDamageFlag(bool flg);

	//攻撃回数をリセット
	void ResetAttackCount(void);

	//モードチェンジ
	void ChangeMode(const MODE& mode);
	MODE GetMode(void)const;

	//当たり判定フラグ
	bool CollFlag(void);

protected:

	//攻撃ヒット時の画面揺れ回数
	static constexpr int ATTACK_VIBRATION_CNT = 15;
	//カウンター攻撃の攻撃範囲
	static constexpr int COUNTER_ATTACK_AREA = 400;
	//回避時のスタミナ消費量
	static constexpr float SUB_STAMINA_AVOID = 30.0f;
	//ジャンプ時のスタミナ消費量
	static constexpr float SUB_STAMINA_JUMP = 10.0f;
	//高速移動時のスタミナ消費量
	static constexpr float SUB_STAMINA_QUICKMOVE = 50.0f;
	//回避力の最大値
	static constexpr float AVOID_POWER_MAX = 100.0f;
	//回避力の最小値
	static constexpr float AVOID_POWER_MIN = 40.0f;

	using UpdateFunc = std::function<void(void)>;

	//フレーム速度
	float frameSpeed_;

	MODE mode_;
	UpdateFunc updateFunc_;

	//デバッグ用の時間
	std::map<TIME, float>timeCounter_;

	//AIにより操る対象
	std::weak_ptr<ActorBase>actor_;

	//移動しているか
	bool moveFlag_;
	//移動方向
	Dir3DF moveDir_;
	//コマンド：弱攻撃の回数を記録
	int atackWeakCnt_;
	//コマンド：強攻撃の回数を記録
	int atackStrongCnt_;
	//コマンド処理しているか
	bool comandFlag_;
	//回避方向
	Dir3DF avoidDir_;
	//前回の回避方向
	Dir3DF prevAvoidDir_;
	//ジャンプしているか
	bool jumpFlag_;
	
	//当たり判定フラグ
	bool colliderFlag_;
	//ジャスト判定に使う
	bool damageFlag_;



	//内部でのみ使用する関数
	void InitTime(void);
	void UpdateTime(void);
	void UpdateAuto(void);
	void UpdateManual(void);
	void CreateNode(void)override;
	void ConnectNode(void)override;
	//条件付きで初期化する
	bool ParamInitStaminaRecove(void);
	//条件分岐
	int ConditionBranch(void)override;
	//移動条件
	void ConditionMove(int& idx);
	//コマンド入力条件
	void ConditionComand(int& idx);
	//対象アクターが敵アクターのいる方向に向く
	Dir3DF FactEnemy(void);

	//ノードに登録する用の関数
	//自動：遠距離
	int ConditionAutoMoveFar(void)const;
	//自動：中距離
	int ConditionAutoMoveMedium(void)const;
	//自動：ガード
	int ConditionAutoDefence(void)const;
	//自動：回避
	int ConditionAutoAvoid(void)const;
	//自動：逃げ走り
	int ConditionAutoRun(void)const;
	//自動：弱攻撃
	int ConditionAutoWeek(void)const;
	//自動：弱・強攻撃
	int ConditionAutoStrong(void)const;

	//上
	int ConditionMoveUp(void)const;
	//下
	int ConditionMoveDown(void)const;
	//左
	int ConditionMoveLeft(void)const;
	//右
	int ConditionMoveRight(void)const;
	//左上
	int ConditionMoveLeftUp(void)const;
	//右上
	int ConditionMoveRightUp(void)const;
	//左下
	int ConditionMoveLeftDown(void)const;
	//右下
	int ConditionMoveRightDown(void)const;

	//弱攻撃
	int ConditionComandWeek(void)const;
	//弱攻撃分岐
	int ConditionComandWeekBranch(void);
	//強攻撃
	int ConditionComandStrong(void)const;
	//強攻撃分岐
	int ConditionComandStrongBranch(void);
	//回避行動
	int ConditionComandAvoid(void)const;
	//高速移動
	int ConditionComandQuickMove(void)const;
	//ジャンプ
	int ConditionComandJump(void)const;
	//落下攻撃
	int ConditionComandFallAttack(void)const;
	//防御
	int ConditionComandDefence(void)const;
	//必殺技
	int ConditionComandUnleashSkill(void)const;
	//カウンターアタック
	int ConditionComandCounterAttack(void);

	//アクションの実態
	//自動：遠距離
	void ActionAutoMoveFar(void);
	//自動：中距離
	void ActionAutoMoveMedium(void);
	//自動：ガード
	void ActionAutoDefence(void);
	//自動：回避
	void ActionAutoAvoid(void);
	//自動：逃げ走り
	void ActionAutoRun(void);
	//自動：カウンターアタック
	void ActionAutoCounterAttack(void);

	//移動アクション
	//上
	void ActionMoveUp(void);
	//下
	void ActionMoveDown(void);
	//左
	void ActionMoveLeft(void);
	//右
	void ActionMoveRight(void);
	//左上
	void ActionMoveLeftUp(void);
	//右上
	void ActionMoveRightUp(void);
	//左下
	void ActionMoveLeftDown(void);
	//右下
	void ActionMoveRightDown(void);

	//コマンドアクション
	//弱攻撃1
	virtual void ActionComandWeek_1(void);
	//弱攻撃2
	virtual void ActionComandWeek_2(void);
	//弱攻撃3
	virtual void ActionComandWeek_3(void);
	//弱攻撃4
	virtual void ActionComandWeek_4(void);
	//弱攻撃5
	virtual void ActionComandWeek_5(void);
	//弱攻撃6
	virtual void ActionComandWeek_6(void);
	//強攻撃1
	virtual void ActionComandStrong_1(void);
	//強攻撃2
	virtual void ActionComandStrong_2(void);
	//強攻撃3
	virtual void ActionComandStrong_3(void);
	//回避行動
	void ActionComandAvoid(void);
	//高速移動
	void ActionComandQuickMove(void);
	//ジャンプ
	void ActionComandJump(void);
	//落下攻撃
	void ActionComandFallAttack(void);
	//防御
	void ActionComandDefence(void);
	//必殺技
	virtual void ActionComandUnleashSkill(void);
	//カウンターアタック
	void ActionComandCounterAttack(void);
	//カウンターアタック失敗
	void ActionComandFailCounterAttack(void);

};