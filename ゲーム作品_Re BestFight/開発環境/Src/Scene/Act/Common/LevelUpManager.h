#pragma once
#include<memory>
#include<map>
#include<functional>

class UserInterface;

class LevelUpManager
{
public:

	//上げるパラメータのID
	//各パラメータの上がり具合をIDで分ける
	enum class LEVELUP_ID
	{
		ATTACK,
		DEFENCE,
		AVOID,
		JUST,
		QUICK_MOVE,
	};

	//コンストラクタ
	LevelUpManager(std::weak_ptr<UserInterface>userInterface);
	//デストラクタ
	~LevelUpManager(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//メモリ開放
	void Release(void);

	//条件を満たしたレベルアップIDをセット
	void MeetCondision(LEVELUP_ID id);

private:

	using SplitFuncsData = std::map<LEVELUP_ID, std::function<void(void)>>;

	//レベルごとに分割して与える経験値計算
	SplitFuncsData spilitValues_;
	//レベルアップ条件を満たしたか
	std::map<LEVELUP_ID, bool>meetCondisions_;
	
	//共有するUIデータ
	std::weak_ptr<UserInterface>userInterface_;


	//内部でしか使わない関数
	void InitSpilitValues(void);
	void InitMeetCondisions(void);
	//初期化用の何もしない関数
	void SpilitFunc();
	//攻撃による経験値の割り振り
	void AttackExAllot(void);
	//防御による経験値の割り振り
	void DefenceExAllot(void);
	//回避による経験値の割り振り
	void AvoidExAllot(void);
	//ジャスト判定による経験値の割り振り
	void JustExAllot(void);
	//高速移動による経験値の割り振り
	void QuickMoveExAllot(void);

};