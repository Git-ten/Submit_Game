#include"../../../../Utility/MyUtility.h"
#include"Node/NodeBase.h"
#include"Node/ActionNode.h"
#include"Node/ConditionNode.h"
#include"Node/Edge.h"
#include"../../ActorBase.h"
#include"../../Common/Userinterface.h"
#include"../../Common/LevelUpManager.h"
#include"../../Common/AnimationController.h"
#include"../../../Common/Collider.h"
#include"../../../../Manager/SoundManager.h"
#include"../../../../Manager/InputManager.h"
#include"../../../Camera.h"
#include<DxLib.h>
#include<cmath>
#include "ActorAi.h"

static InputManager& input = InputManager::GetInstance();

ActorAi::ActorAi(std::weak_ptr<ActorBase> actor)
{
	frameSpeed_ = DEFAULT_FRAME_SPEED;
	ChangeMode(MODE::MANUAL);
	actor_ = actor;
	nowIndex_ = NONE_INDEX;
	nowCost_ = NONE_COST;
	//当たり判定フラグ
	colliderFlag_ = false;
	//移動しているか
	moveFlag_ = false;
	//移動方向
	moveDir_ = MyUtility::VECTOR_ZERO;
	//コマンド：弱攻撃の回数を記録
	atackWeakCnt_ = 0;
	atackStrongCnt_ = 0;
	//コマンド処理しているか
	comandFlag_ = false;
	//回避方向
	avoidDir_ = MyUtility::VECTOR_ZERO;
	//前回の回避方向
	prevAvoidDir_ = MyUtility::VECTOR_ZERO;
	//ジャンプしているか
	jumpFlag_ = false;
	//ジャスト判定に使う
	damageFlag_ = false;

	InitTime();//時間の初期化
}

ActorAi::~ActorAi()
{

}

void ActorAi::Update()
{
	if (timeCounter_.at(TIME::FRAME_TIME) <= 0.0f)
	{
		updateFunc_();

		//変数初期化
		SetTime(TIME::FRAME_TIME, DEFAULT_FRAME_TIME);
		actor_.lock()->SetUpdateFlag(true);
	}
	else
	{
		actor_.lock()->SetUpdateFlag(false);
	}

	UpdateTime();
}

void ActorAi::SetTime(const TIME& type, float t)
{
	const auto& ite = timeCounter_.find(type);
	if (ite != timeCounter_.end())
	{
		timeCounter_.at(type) = t;
	}
}

void ActorAi::SetFrameSpeed(float speed)
{
	frameSpeed_ = speed;
}

void ActorAi::SetFrameDefaultSpeed()
{
	frameSpeed_ = DEFAULT_FRAME_SPEED;
}

bool ActorAi::GetDamageFlag() const
{
	return damageFlag_;
}

void ActorAi::SetDamageFlag(bool flg)
{
	damageFlag_ = flg;
}

void ActorAi::ResetAttackCount()
{
	atackWeakCnt_ = 0;
	atackStrongCnt_ = 0;
}

void ActorAi::ChangeMode(const MODE& mode)
{
	mode_ = mode;
	switch (mode_)
	{
	case MODE::AUTO:
		updateFunc_ = [&]() {UpdateAuto(); };
		break;
	case MODE::MANUAL:
		updateFunc_ = [&]() {UpdateManual(); };
		break;
	default:
		break;
	}
}

ActorAi::MODE ActorAi::GetMode() const
{
	return mode_;
}

bool ActorAi::CollFlag(void)
{
	return colliderFlag_;
}

void ActorAi::InitTime()
{
	timeCounter_.clear();
	timeCounter_.emplace(TIME::FRAME_TIME, 0.0f);
	timeCounter_.emplace(TIME::ATTACK_RAG, 0.0f);
	timeCounter_.emplace(TIME::AVOID_TIME, 0.0f);
	timeCounter_.emplace(TIME::QUICK_TIME, 0.0f);
	timeCounter_.emplace(TIME::QUICK_RAG, 0.0f);
	timeCounter_.emplace(TIME::JUST_RECEP_TIME, 0.0f);
	timeCounter_.emplace(TIME::JUST_CHANCE_TIME, 0.0f);
	timeCounter_.emplace(TIME::GURD_RAG, 0.0f);
	timeCounter_.emplace(TIME::COUNTER_RAG, 0.0f);
}

void ActorAi::UpdateTime()
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	for (auto& t : timeCounter_)
	{
		if (t.second > 0.0f)
		{
			t.second -= delta * frameSpeed_;
		}
		else
		{
			//初期化
			t.second = 0.0f;
		}
	}
}

void ActorAi::UpdateAuto()
{
	if (nowIndex_ != NONE_INDEX)
	{
		NodeTransition(nowIndex_, nowIndex_);

		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			//終端ノードの場合のみ処理
			if (nodes_[nSearch]->IsEndNode())
			{
				//アクションが終了
				if (nodes_[nSearch]->IsEndAction())
				{
					nodes_[nSearch]->SetIsEndAction(false);
				}
				else
				{
					nodes_[nSearch]->Update();
				}
			}
		}
	}

	ParamInitStaminaRecove();
}

void ActorAi::UpdateManual()
{
	//実行可能なノードの探索
	int sCounter = 0;
	while (sCounter < MAX_NODE_SEARCH)
	{
		nowIndex_ = ConditionBranch();
		if (ParamInitStaminaRecove())break;

		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			//終端ノードの場合のみ処理
			if (nodes_[nSearch]->IsEndNode())
			{
				//アクションが終了
				if (nodes_[nSearch]->IsEndAction())
				{
					nodes_[nSearch]->SetIsEndAction(false);
				}
				else
				{
					nodes_[nSearch]->Update();
				}
				break;
			}
		}

		sCounter++;
	}
}

void ActorAi::CreateNode()
{
	//移動系ノード-------------------------------
	//手動
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_UP), [&]() {return ConditionMoveUp(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_DOWN), [&]() {return ConditionMoveDown(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_LEFT), [&]() {return ConditionMoveLeft(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_RIGHT), [&]() {return ConditionMoveRight(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_LEFTUP), [&]() {return ConditionMoveLeftUp(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_RIGHTUP), [&]() {return ConditionMoveRightUp(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_LEFTDOWN), [&]() {return ConditionMoveLeftDown(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN), [&]() {return ConditionMoveRightDown(); });

	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_UP), [&]() {ActionMoveUp(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_DOWN), [&]() {ActionMoveDown(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_LEFT), [&]() {ActionMoveLeft(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_RIGHT), [&]() {ActionMoveRight(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_LEFTUP), [&]() {ActionMoveLeftUp(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_RIGHTUP), [&]() {ActionMoveRightUp(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN), [&]() {ActionMoveLeftDown(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN), [&]() {ActionMoveRightDown(); });


	//自動
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_MOVE_FAR), [&]() {return ConditionAutoMoveFar(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_MOVE_MEDIUM), [&]() {return ConditionAutoMoveMedium(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_DEFENCE), [&]() {return ConditionAutoDefence(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_AVOID), [&]() {return ConditionAutoAvoid(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_RUN), [&]() {return ConditionAutoRun(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_WEEK), [&]() {return ConditionAutoWeek(); });
	AddConditionNode(INT_INDEX(INDEX_CON_AUTO_STRONG), [&]() {return ConditionAutoStrong(); });

	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_MOVE_FAR), [&]() {ActionAutoMoveFar(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM), [&]() {ActionAutoMoveMedium(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_DEFENCE), [&]() {ActionAutoDefence(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_AVOID), [&]() {ActionAutoAvoid(); });
	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_RUN), [&]() {ActionAutoRun(); });

	AddActionNode(INT_INDEX(INDEX_ACT_AUTO_COUNTER), [&]() {ActionAutoCounterAttack(); });
	//--------------------------------------------

	//コマンド系ノード----------------------------
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_WEEK), [&]() {return ConditionComandWeek(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH), [&]() {return ConditionComandWeekBranch(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_STRONG), [&]() {return ConditionComandStrong(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH), [&]() {return ConditionComandStrongBranch(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_AVOID), [&]() {return ConditionComandAvoid(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_QUICKMOVE), [&]() {return ConditionComandQuickMove(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_JUMP), [&]() {return ConditionComandJump(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_FALLATTACK), [&]() {return ConditionComandFallAttack(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_DEFENCE), [&]() {return ConditionComandDefence(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_UNLEASHSKILL), [&]() {return ConditionComandUnleashSkill(); });
	AddConditionNode(INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK), [&]() {return ConditionComandCounterAttack(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_1), [&]() {ActionComandWeek_1(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_2), [&]() {ActionComandWeek_2(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_3), [&]() {ActionComandWeek_3(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_4), [&]() {ActionComandWeek_4(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_5), [&]() {ActionComandWeek_5(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_WEEK_6), [&]() {ActionComandWeek_6(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_STRONG_1), [&]() {ActionComandStrong_1(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_STRONG_2), [&]() {ActionComandStrong_2(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_STRONG_3), [&]() {ActionComandStrong_3(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_AVOID), [&]() {ActionComandAvoid(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE), [&]() {ActionComandQuickMove(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_JUMP), [&]() {ActionComandJump(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_FALLATTACK), [&]() {ActionComandFallAttack(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_DEFENCE), [&]() {ActionComandDefence(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_UNLEASHSKILL), [&]() {ActionComandUnleashSkill(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK), [&]() {ActionComandCounterAttack(); });
	AddActionNode(INT_INDEX(INDEX_ACT_COMAND_FAIL_COUNTERATTACK), [&]() {ActionComandFailCounterAttack(); });
	//--------------------------------------------
}

void ActorAi::ConnectNode()
{
	//自動
	//遠距離移動条件ー＞遠距離移動処理
	int from = INT_INDEX(INDEX_CON_AUTO_MOVE_FAR);
	int to = INT_INDEX(INDEX_ACT_AUTO_MOVE_FAR);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//中距離移動条件ー＞中距離移動処理
	from = INT_INDEX(INDEX_CON_AUTO_MOVE_MEDIUM);
	to = INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//ガード条件ー＞ガード処理
	from = INT_INDEX(INDEX_CON_AUTO_DEFENCE);
	to = INT_INDEX(INDEX_ACT_AUTO_DEFENCE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//回避条件ー＞回避処理
	from = INT_INDEX(INDEX_CON_AUTO_AVOID);
	to = INT_INDEX(INDEX_ACT_AUTO_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//走る条件ー＞走る処理
	from = INT_INDEX(INDEX_CON_AUTO_RUN);
	to = INT_INDEX(INDEX_ACT_AUTO_RUN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//自動弱攻撃条件ー＞自動弱攻撃処理
	from = INT_INDEX(INDEX_CON_AUTO_WEEK);
	to = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//自動弱攻撃条件ー＞自動ジャンプ処理
	from = INT_INDEX(INDEX_CON_AUTO_WEEK);
	to = INT_INDEX(INDEX_ACT_COMAND_JUMP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//自動強攻撃条件ー＞自動強攻撃処理
	from = INT_INDEX(INDEX_CON_AUTO_STRONG);
	to = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//手動
	//移動条件＆アクションノードのエッジ
	//上方向移動条件ー＞上方向移動
	from = INT_INDEX(INDEX_CON_MOVE_UP);
	to = INT_INDEX(INDEX_ACT_MOVE_UP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//下方向移動条件ー＞下方向移動
	from = INT_INDEX(INDEX_CON_MOVE_DOWN);
	to = INT_INDEX(INDEX_ACT_MOVE_DOWN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//左方向移動条件ー＞左方向
	from = INT_INDEX(INDEX_CON_MOVE_LEFT);
	to = INT_INDEX(INDEX_ACT_MOVE_LEFT);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//右方向移動条件ー＞右方向
	from = INT_INDEX(INDEX_CON_MOVE_RIGHT);
	to = INT_INDEX(INDEX_ACT_MOVE_RIGHT);
	AddConnectEdge(from, to, DEFAULT_NODE_COST);

	//左上方向移動条件ー＞左上方向
	from = INT_INDEX(INDEX_CON_MOVE_LEFTUP);
	to = INT_INDEX(INDEX_ACT_MOVE_LEFTUP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);

	//右上方向移動条件ー＞右上方向
	from = INT_INDEX(INDEX_CON_MOVE_RIGHTUP);
	to = INT_INDEX(INDEX_ACT_MOVE_RIGHTUP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);

	//左下方向移動条件ー＞左下方向
	from = INT_INDEX(INDEX_CON_MOVE_LEFTDOWN);
	to = INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);

	//右下方向移動条件ー＞右下方向
	from = INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN);
	to = INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 1.0f);


	//コマンド入力＆アクションノードのエッジ
	//弱攻撃コマンド入力条件ー＞弱攻撃分岐
	from = INT_INDEX(INDEX_CON_COMAND_WEEK);
	to = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.0f);

	//弱攻撃分岐ー＞弱攻撃1
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_1);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//弱攻撃分岐ー＞弱攻撃2
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_2);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.2f);

	//弱攻撃分岐ー＞弱攻撃3
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_3);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.3f);

	//弱攻撃分岐ー＞弱攻撃4
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_4);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.4f);

	//弱攻撃分岐ー＞弱攻撃5
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_5);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.5f);

	//弱攻撃分岐ー＞弱攻撃6
	from = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_WEEK_6);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.6f);

	//強攻撃コマンド入力条件ー＞強攻撃分岐
	from = INT_INDEX(INDEX_CON_COMAND_STRONG);
	to = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.0f);

	//強攻撃分岐ー＞強攻撃1
	from = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_STRONG_1);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//強攻撃分岐ー＞強攻撃2
	from = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_STRONG_2);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.2f);

	//強攻撃分岐ー＞強攻撃3
	from = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	to = INT_INDEX(INDEX_ACT_COMAND_STRONG_3);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.3f);

	//回避コマンド入力条件ー＞回避
	from = INT_INDEX(INDEX_CON_COMAND_AVOID);
	to = INT_INDEX(INDEX_ACT_COMAND_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.5f);

	//高速移動コマンド入力条件ー＞高速移動
	from = INT_INDEX(INDEX_CON_COMAND_QUICKMOVE);
	to = INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.6f);

	//ジャンプコマンド入力条件ー＞ジャンプ
	from = INT_INDEX(INDEX_CON_COMAND_JUMP);
	to = INT_INDEX(INDEX_ACT_COMAND_JUMP);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.5f);

	//落下攻撃コマンド入力条件ー＞落下攻撃
	from = INT_INDEX(INDEX_CON_COMAND_FALLATTACK);
	to = INT_INDEX(INDEX_ACT_COMAND_FALLATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//防御コマンド入力条件ー＞防御
	from = INT_INDEX(INDEX_CON_COMAND_DEFENCE);
	to = INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.7f);

	//防御ー＞回避
	from = INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
	to = INT_INDEX(INDEX_ACT_COMAND_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.8f);

	//必殺技コマンド入力条件ー＞必殺技
	from = INT_INDEX(INDEX_CON_COMAND_UNLEASHSKILL);
	to = INT_INDEX(INDEX_ACT_COMAND_UNLEASHSKILL);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 3.0f);

	//カウンター攻撃コマンド入力条件ー＞カウンター攻撃
	from = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
	to = INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);

	//カウンター攻撃コマンド入力条件ー＞カウンター攻撃失敗
	from = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
	to = INT_INDEX(INDEX_ACT_COMAND_FAIL_COUNTERATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 2.1f);
}

bool ActorAi::ParamInitStaminaRecove()
{
	//スタミナ回復条件
	if (nowIndex_ == NONE_INDEX ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_UP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_DOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_AUTO_RUN))
	{
		actor_.lock()->userInterface_->AddStamina(0.3f);
	}

	if (nowIndex_ == NONE_INDEX)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::IDLE)
		{
			moveFlag_ = false;
			moveDir_ = MyUtility::VECTOR_ZERO;
			comandFlag_ = false;
			avoidDir_ = MyUtility::VECTOR_ZERO;
			prevAvoidDir_ = MyUtility::VECTOR_ZERO;
			jumpFlag_ = false;
			actor_.lock()->ChangeState(ActorBase::STATE::IDLE);
			return true;
		}
	}

	return false;
}

int ActorAi::ConditionBranch()
{
	int ans = nowIndex_;

	if (mode_ == MODE::MANUAL)
	{
		//移動条件判定
		ConditionMove(ans);
		//コマンド入力条件判定
		ConditionComand(ans);
	}

	return ans;
}

void ActorAi::ConditionMove(int& idx)
{
	if (nowIndex_ == NONE_INDEX)
	{
		//上移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_UP), idx);
		//下移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_DOWN), idx);
		//左移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_LEFT), idx);
		//右移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_RIGHT), idx);
		//左上移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_LEFTUP), idx);
		//右上移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_RIGHTUP), idx);
		//左下移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_LEFTDOWN), idx);
		//右下移動
		NodeTransition(INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN), idx);
	}
}

void ActorAi::ConditionComand(int& idx)
{
	if (nowIndex_ == NONE_INDEX ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_UP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_DOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHT) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTUP) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN) ||
		nowIndex_ == INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN))
	{
		//弱攻撃
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_WEEK), idx);
		//強攻撃
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_STRONG), idx);
		//回避
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_AVOID), idx);
		//高速移動
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_QUICKMOVE), idx);
		//ジャンプ
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_JUMP), idx);
		//防御
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_DEFENCE), idx);
		//必殺技
		NodeTransition(INT_INDEX(INDEX_CON_COMAND_UNLEASHSKILL), idx);
	}
	else
	{
		//条件分岐
		NodeTransition(nowIndex_, idx);
	}
}

Dir3DF ActorAi::FactEnemy()
{
	Dir3DF dir = VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos);
	dir = VNorm(dir);
	Quaternion q;
	Rot3DF angle = q.PosAxis(dir);
	//ロール回転は無効
	angle.y = 0.0f;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);

	return dir;
}

int ActorAi::ConditionAutoMoveFar() const
{
	//条件チェック
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis >= std::powf(DISTANCE_FAR, 2.0f))
	{
		return INT_INDEX(INDEX_ACT_AUTO_MOVE_FAR);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionAutoMoveMedium() const
{
	//条件チェック
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis >= std::powf(DISTANCE_MEDIUM, 2.0f))
	{
		return INT_INDEX(INDEX_ACT_AUTO_MOVE_MEDIUM);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionAutoDefence() const
{
	return INT_INDEX(INDEX_ACT_AUTO_DEFENCE);
}

int ActorAi::ConditionAutoAvoid() const
{
	return INT_INDEX(INDEX_ACT_AUTO_AVOID);
}

int ActorAi::ConditionAutoRun() const
{
	return INT_INDEX(INDEX_ACT_AUTO_RUN);
}

int ActorAi::ConditionAutoWeek() const
{
	if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
	{
		bool random = static_cast<bool>(GetRand(100) % 100);
		if (!random)
		{
			return INT_INDEX(INDEX_ACT_COMAND_JUMP);
		}
		return INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionAutoStrong() const
{
	if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
	{
		return INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	}
	return NONE_INDEX;
}

//移動条件
int ActorAi::ConditionMoveUp()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_UP);
	}

	return NONE_INDEX;
}

int ActorAi::ConditionMoveDown()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT))
	{
		return INT_INDEX(INDEX_ACT_MOVE_DOWN);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveLeft()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_LEFT);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveRight()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_RIGHT);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveLeftUp()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_LEFTUP);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveRightUp()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_UP) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_RIGHTUP);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveLeftDown()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_LEFT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_LEFTDOWN);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionMoveRightDown()const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_RIGHT) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DOWN) &&
		!input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		return INT_INDEX(INDEX_ACT_MOVE_RIGHTDOWN);
	}

	return NONE_INDEX;
}

//コマンド入力条件
int ActorAi::ConditionComandWeek() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_WEEKATC))
	{
		return INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandWeekBranch()
{
	if (jumpFlag_)
	{
		return INT_INDEX(INDEX_CON_COMAND_FALLATTACK);
	}
	//攻撃回数0回
	else if (atackWeakCnt_ <= 0)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_1);
	}
	//攻撃回数1回
	else if (atackWeakCnt_ <= 1)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_2);
	}
	//攻撃回数2回
	else if (atackWeakCnt_ <= 2)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_3);
	}
	//攻撃回数3回
	else if (atackWeakCnt_ <= 3)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_4);
	}
	//攻撃回数4回
	else if (atackWeakCnt_ <= 4)
	{
		atackWeakCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_5);
	}
	//攻撃回数5回
	else if (atackWeakCnt_ <= 5)
	{
		atackWeakCnt_ = 0;//回数リセット
		return INT_INDEX(INDEX_ACT_COMAND_WEEK_6);
	}

	return NONE_INDEX;
}

int ActorAi::ConditionComandStrong() const
{
	if(input.GetSimpleInput(SimpleInput::SIMPLE::KEY_STRONGATC))
	{ 
	return INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandStrongBranch()
{
	if (jumpFlag_)
	{
		return INT_INDEX(INDEX_CON_COMAND_FALLATTACK);
	}
	//攻撃回数0回
	else if (atackStrongCnt_ <= 0)
	{
		atackStrongCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_STRONG_1);
	}
	//攻撃回数1回
	else if (atackStrongCnt_ <= 1)
	{
		atackStrongCnt_++;
		return INT_INDEX(INDEX_ACT_COMAND_STRONG_2);
	}
	//攻撃回数2～5回
	else if (atackStrongCnt_ <= 2)
	{
		atackStrongCnt_ = 0;//リセット
		return INT_INDEX(INDEX_ACT_COMAND_STRONG_3);
	}

	return NONE_INDEX;
}

int ActorAi::ConditionComandAvoid() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		if (actor_.lock()->userInterface_->GetStateStamina() >= SUB_STAMINA_AVOID)
		{
			return INT_INDEX(INDEX_ACT_COMAND_AVOID);
		}
		else
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandQuickMove() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_QUICKMOVE) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_AVOID))
	{
		if (actor_.lock()->userInterface_->GetStateStamina() >= SUB_STAMINA_QUICKMOVE)
		{
			return INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE);
		}
		else
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandJump() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_JUMP))
	{
		if (actor_.lock()->userInterface_->GetStateStamina() >= SUB_STAMINA_JUMP)
		{
			return INT_INDEX(INDEX_ACT_COMAND_JUMP);
		}
		else
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandFallAttack() const
{
	if ((input.GetSimpleInput(SimpleInput::SIMPLE::KEY_WEEKATC) ||
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_STRONGATC)))
	{
		return INT_INDEX(INDEX_ACT_COMAND_FALLATTACK);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandDefence() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_DEFENCE) &&
		timeCounter_.at(TIME::GURD_RAG) <= 0.0f)
	{
		return INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandUnleashSkill() const
{
	if (input.GetSimpleInput(SimpleInput::SIMPLE::KEY_SKILL_1) &&
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_SKILL_2) &&
		actor_.lock()->IsUnleashSkill())
	{
		return INT_INDEX(INDEX_ACT_COMAND_UNLEASHSKILL);
	}
	return NONE_INDEX;
}

int ActorAi::ConditionComandCounterAttack()
{
	if (timeCounter_.at(TIME::JUST_CHANCE_TIME) > 0.0f &&
		(input.GetSimpleInput(SimpleInput::SIMPLE::KEY_WEEKATC) ||
		input.GetSimpleInput(SimpleInput::SIMPLE::KEY_STRONGATC)))
	{
		//相手のフレーム速度を元に戻す
		actor_.lock()->enemy_.lock()->actAi_->SetFrameDefaultSpeed();
		return INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK);
	}
	else if(timeCounter_.at(TIME::JUST_CHANCE_TIME) <= 0.0f)
	{
		//相手のフレーム速度を元に戻す
		actor_.lock()->enemy_.lock()->actAi_->SetFrameDefaultSpeed();
		actor_.lock()->actorCam_->ChangeDefaultMode();
		return INT_INDEX(INDEX_ACT_COMAND_FAIL_COUNTERATTACK);
	}

	return NONE_INDEX;
}

void ActorAi::ActionAutoMoveFar()
{
	if (timeCounter_.at(TIME::QUICK_RAG) <= 0.0f &&
		nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_QUICKMOVE);
		nowCost_ = NONE_COST;
	}
	else
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MOVE)
		{
			actor_.lock()->ChangeState(ActorBase::STATE::MOVE);
		}
		else
		{
			//敵の方角に向く
			Dir3DF dir = FactEnemy();
			dir = VScale(dir, -1.0f);

			//敵の方角に移動
			actor_.lock()->transform_->pos = VAdd(
				actor_.lock()->transform_->pos,
				VScale(dir, actor_.lock()->GetSpeed()));

			//前提条件を満たさなくなったらアクション終了
			int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_AUTO_MOVE_FAR));
			if (nSearch >= 0)
			{
				if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
				{
					EndNodeAction(nSearch);
				}
			}
		}
	}

}

void ActorAi::ActionAutoMoveMedium()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::MOVE)
	{
		actor_.lock()->ChangeState(ActorBase::STATE::MOVE);
	}
	else
	{
		//移動処理
		//敵の方角に向く
		Dir3DF dir = FactEnemy();
		dir = VScale(dir, -1.0f);

		actor_.lock()->transform_->pos = VAdd(
			actor_.lock()->transform_->pos,
			VScale(dir, actor_.lock()->GetSpeed()));

		//前提条件を満たさなくなったらアクション終了
		int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_AUTO_MOVE_MEDIUM));
		if (nSearch >= 0)
		{
			if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionAutoDefence()
{
	if (nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_DEFENCE))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_DEFENCE);
		nowCost_ = NONE_COST;
	}
}

void ActorAi::ActionAutoAvoid()
{
	if (nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_AVOID))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_AVOID);
		nowCost_ = NONE_COST;
	}
}

void ActorAi::ActionAutoRun()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::MOVE)
	{
		actor_.lock()->ChangeState(ActorBase::STATE::MOVE);
	}
	else
	{

		//敵の方角に向く
		Dir3DF dir = FactEnemy();
		dir = VScale(dir, -1.0f);

		actor_.lock()->transform_->pos = VAdd(
			actor_.lock()->transform_->pos,
			VScale(dir, actor_.lock()->GetSpeed()));

		//一応インデックスのサーチをしてから無条件でアクション終了
		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			EndNodeAction(nSearch);
		}
	}
}

void ActorAi::ActionAutoCounterAttack(void)
{
	if (timeCounter_.at(TIME::COUNTER_RAG) <= 0.0f && nowIndex_ != INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK))
	{
		nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_COUNTERATTACK);
		nowCost_ = NONE_COST;
	}
}

//移動アクション
void ActorAi::ActionMoveUp()
{
	//transform_->pos.z += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetForward();

	//モデルの元の前方向が-z軸であるため180度回転させる
	Quaternion q = Quaternion::AngleAxis(MyUtility::Deg2RadD(180.0f), MyUtility::AXIS_Y);
	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;
	
	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_UP));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionMoveDown()
{
	//transform_->pos.z -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetBack();

	Quaternion q = Quaternion();
	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_DOWN));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
				
			}
		}
	}
}

void ActorAi::ActionMoveLeft()
{
	//transform_->pos.x -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();

	Quaternion q = Quaternion();
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(90.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_LEFT));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionMoveRight()
{
	//transform_->pos.x += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();

	//モデルの元の前方向が-z軸であるため180度回転させる
	Quaternion q = Quaternion();

	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-90.0f), MyUtility::AXIS_Y));
	

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_RIGHT));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveLeftUp()
{
	//transform_->pos.x -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetForward());

	Quaternion q = Quaternion();

	//上方向
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(135.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_LEFTUP));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveRightUp()
{
	//transform_->pos.x += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetForward());

	//モデルの元の前方向が-z軸であるため180度回転させる
	Quaternion q = Quaternion();

	//上方向も押下している場合
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-135.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_RIGHTUP));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveLeftDown()
{
	//transform_->pos.x -= 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetBack());

	Quaternion q = Quaternion();
	//下方向
	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(45.0f), MyUtility::AXIS_Y));


	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_LEFTDOWN));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);	
			}
		}
	}
}

void ActorAi::ActionMoveRightDown()
{
	//transform_->pos.x += 10.0f;
	Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();
	dir = VAdd(dir, actor_.lock()->camera_.lock()->GetBack());

	//モデルの元の前方向が-z軸であるため180度回転させる
	Quaternion q = Quaternion();

	q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-45.0f), MyUtility::AXIS_Y));

	Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
	//ロール回転は無効
	angle.y = 0.0f;

	moveFlag_ = true;
	//移動方向
	moveDir_ = dir;
	actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
	actor_.lock()->ChangeState(ActorBase::STATE::MOVE);

	//前提条件が満たされなくなったとき
	//アクションを終了
	int nSearch = SearchNodeIdx(INT_INDEX(INDEX_CON_MOVE_RIGHTDOWN));
	if (nSearch >= 0)
	{
		if (nodes_[nSearch]->IsCondition() == NONE_INDEX)
		{
			//一応インデックスのサーチをしてからアクション終了
			nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

//コマンドアクション
void ActorAi::ActionComandWeek_1() 
{

}

void ActorAi::ActionComandWeek_2()
{

}

void ActorAi::ActionComandWeek_3()
{

}

void ActorAi::ActionComandWeek_4()
{

}

void ActorAi::ActionComandWeek_5()
{

}

void ActorAi::ActionComandWeek_6()
{

}

void ActorAi::ActionComandStrong_1() 
{

}

void ActorAi::ActionComandStrong_2()
{

}

void ActorAi::ActionComandStrong_3()
{
	
}

void ActorAi::ActionComandAvoid()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::AVOID &&
		actor_.lock()->GetState() != ActorBase::STATE::BACKSTEP)
	{
		SetTime(TIME::JUST_RECEP_TIME, 0.05f *
			static_cast<float>(actor_.lock()->userInterface_->GetStateAvoidLevel()));

		comandFlag_ = false;

		if (ConditionMoveLeft() == NONE_INDEX && ConditionMoveRight() == NONE_INDEX &&
			ConditionMoveDown() == NONE_INDEX)
		{
			//どこにも移動キーを押してなければ状態をバックステップにする
			actor_.lock()->ChangeState(ActorBase::STATE::BACKSTEP);
		}
		else
		{
			actor_.lock()->ChangeState(ActorBase::STATE::AVOID);
		}

		//スタミナを消費
		actor_.lock()->userInterface_->SubStamina(SUB_STAMINA_AVOID);
		actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
	}
	else
	{
		//回避処理
		//1フレーム過ぎてから回避行動をとるようにしている
		if (!comandFlag_)
		{
			comandFlag_ = true;
			float power = actor_.lock()->GetAvoidPower();

			if (ConditionMoveLeft() != NONE_INDEX)
			{
				Dir3DF dir = actor_.lock()->camera_.lock()->GetLeft();
				
				Quaternion q = Quaternion();

				//上方向
				q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(135.0f), MyUtility::AXIS_Y));

				Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
				//ロール回転は無効
				angle.y = 0.0f;

				//移動方向
				actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
				//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
				avoidDir_ = dir;
				power *= 0.8f;
				if (power < AVOID_POWER_MIN)power = AVOID_POWER_MIN;
				else if (power > AVOID_POWER_MAX)power = AVOID_POWER_MAX;
			}
			else if (ConditionMoveRight() != NONE_INDEX)
			{
				Dir3DF dir = actor_.lock()->camera_.lock()->GetRight();

				Quaternion q = Quaternion();

				//上方向
				q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-135.0f), MyUtility::AXIS_Y));

				Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
				//ロール回転は無効
				angle.y = 0.0f;

				//移動方向
				actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
				//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
				avoidDir_ = dir;
				power *= 0.8f;
			}
			else if (ConditionMoveDown() != NONE_INDEX)
			{
				Dir3DF dir = actor_.lock()->camera_.lock()->GetBack();

				Quaternion q = Quaternion();

				//上方向
				q = q.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(67.0f), MyUtility::AXIS_Y));

				Rot3DF angle = q.PosAxis(actor_.lock()->camera_.lock()->GetForward());
				//ロール回転は無効
				angle.y = 0.0f;

				//移動方向
				actor_.lock()->transform_->quaRotLocal = Quaternion::LookRotation(angle);
				//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, actor_.lock()->GetSpeed()));
				avoidDir_ = dir;
				power *= 0.8f;
				if (power < AVOID_POWER_MIN)power = AVOID_POWER_MIN;
				else if (power > AVOID_POWER_MAX)power = AVOID_POWER_MAX;
			}
			else
			{
				//どこにも移動していないときはプレイヤーの後ろ側にセットされる
				avoidDir_ = actor_.lock()->actorCam_->GetBack();

				power *= 0.6f;
				if (power < AVOID_POWER_MIN)power = AVOID_POWER_MIN;
				else if (power > AVOID_POWER_MAX)power = AVOID_POWER_MAX;
				//バックステップはジャンプベクトルで表現
				actor_.lock()->SetJumpVector({ 0.0f, power, 0.0f });
			}

			avoidDir_ = VScale(avoidDir_, power);
			prevAvoidDir_ = avoidDir_;
		}
		else if(actor_.lock()->animController_->GetStepAnimTime() > 0.05f)
		{
			//回避ベクトルの方向と回避処理前回避ベクトルの方向が同じになったら回避終了
			if (VDot(VNorm(avoidDir_), VNorm(prevAvoidDir_)) < 0 &&
				actor_.lock()->transform_->pos.y <= 0)
			{
				comandFlag_ = false;
				avoidDir_ = MyUtility::VECTOR_ZERO;
				prevAvoidDir_ = avoidDir_;

				actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::AVOID);
				//自動で相手の方を向く
				FactEnemy();

				//アクションを終了
				int nSearch = SearchNodeIdx(nowIndex_);
				if (nSearch >= 0)
				{
					EndNodeAction(nSearch);
				}
			}

			//回避処理
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, avoidDir_);

			//回避パワーは徐々に下がる
			//回避方向の逆を算出
			VECTOR subVec = VScale(VNorm(avoidDir_), -1.0f);
			subVec = VScale(subVec, 3.0f);

			avoidDir_ = VAdd(avoidDir_, subVec);
		}


		//ジャスト判定
		if (timeCounter_.at(TIME::JUST_RECEP_TIME) > 0.0f && damageFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			damageFlag_ = false;
			SetTime(TIME::JUST_CHANCE_TIME, 1.5f);
			actor_.lock()->actorCam_->ChangeMode(Camera::MODE::JUST);
			actor_.lock()->enemy_.lock()->SetStopFrame(1.5f);
			actor_.lock()->userInterface_->SetDamagePower(0.0f);
			actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::JUST);
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);
			actor_.lock()->SetSwayVector(MyUtility::VECTOR_ZERO);

			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::JUST, ActorBase::USE_EFFECT::JUST, effPos, effRot);

			actor_.lock()->SoundPlay(SoundManager::SOD::JUST, DX_PLAYTYPE_BACK);

			//アクションの終了処理
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
			//カウンターアタック条件に遷移
			if (mode_ == MODE::MANUAL)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
			}
			else
			{
				nowIndex_ = INT_INDEX(INDEX_ACT_AUTO_COUNTER);
				SetTime(TIME::COUNTER_RAG, 0.3f);
			}
			nowCost_ = NONE_COST;
		}
	}
}

void ActorAi::ActionComandQuickMove()
{
	if (actor_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE)
	{
		//状態を高速移動にする
		actor_.lock()->ChangeState(ActorBase::STATE::QUICK_MOVE);
		SetTime(TIME::QUICK_TIME, 0.65f);
		//当たり判定フラグ
		colliderFlag_ = false;

		actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::QUICK_MOVE);
		//エフェクト
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(-1.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		actor_.lock()->PlayEffect(EffectManager::EFF::QUICK_MOVE, ActorBase::USE_EFFECT::QUICK_MOVE, effPos, effRot);
		//スタミナを消費
		actor_.lock()->userInterface_->SubStamina(SUB_STAMINA_QUICKMOVE);
		actor_.lock()->SoundPlay(SoundManager::SOD::QUICK_MOVE, DX_PLAYTYPE_BACK);
	}
	else
	{
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(-1.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		//エフェクトの座標を合わせる
		actor_.lock()->SetEffectPos(EffectManager::EFF::QUICK_MOVE, ActorBase::USE_EFFECT::QUICK_MOVE, effPos);
		//エフェクトの回転を合わせる
		actor_.lock()->SetEffectRot(EffectManager::EFF::QUICK_MOVE, ActorBase::USE_EFFECT::QUICK_MOVE, effRot);

		//当たり判定の発生
		//アニメーションのステップタイムで判断する
		if (!colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < 100; ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->prevPos, dir));

				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//当たり判定フラグ
					colliderFlag_ = true;

					if (actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_QUICKMOVE)
					{
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					}

					//相手のアクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						if (nSearch >= 0)
						{
							actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						}
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					//自分のアクションを終了
					nSearch = SearchNodeIdx(nowIndex_);
					if (nSearch >= 0)
					{
						EndNodeAction(nSearch);
					}
					break;
				}
			}
		}


		//高速移動処理
		//1フレーム過ぎてから高速移動するようにしている
		//アニメーションが5ステップ進んだあとに移動
		if (actor_.lock()->animController_->GetStepAnimTime() > 5.0f)
		{
			//自動で相手の方を向く
			Dir3DF dir = FactEnemy();
			dir = VScale(dir, -1.0f);

			VECTOR quickMoveVec_ = VScale(dir, ActorBase::QUICK_SPEED);
			//高速移動処理
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, quickMoveVec_);
		}
		else if (timeCounter_.at(TIME::QUICK_TIME) <= 0.0f)
		{
			//高速移動タイムが0になったら、もしくは
			//敵に自分の体が当たったら処理終了
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}


void ActorAi::ActionComandJump() 
{
	if (actor_.lock()->GetState() != ActorBase::STATE::JUMP)
	{
		//状態をジャンプにする
		actor_.lock()->ChangeState(ActorBase::STATE::JUMP);
		//スタミナを消費
		actor_.lock()->userInterface_->SubStamina(SUB_STAMINA_JUMP);
		actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
	}
	else
	{
		//移動中ならその方向にジャンプ中も移動する
		if (moveFlag_)
		{
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(moveDir_, actor_.lock()->GetSpeed()));
		}

		//ジャンプ処理
		//1フレーム過ぎてからジャンプするようにしている
		if (!jumpFlag_)
		{
			//ジャンプというより上下方向に働くベクトル
			actor_.lock()->SetJumpVector(VGet(0.0f, actor_.lock()->GetJumpPower(), 0.0f));
			jumpFlag_ = true;

			//エフェクトの発生
			Position3DF effPos = actor_.lock()->transform_->pos;
			Rot3DF effRot = MyUtility::VECTOR_ZERO;
			actor_.lock()->PlayEffect(EffectManager::EFF::JUMP, ActorBase::USE_EFFECT::JUMP, effPos, effRot);
		}
		//落下攻撃の条件をジャンプ中に満たした場合
		else if ((nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_FALLATTACK))]->IsCondition() != NONE_INDEX) ||
				(mode_ == MODE::AUTO && actor_.lock()->animController_->GetStepAnimTime() >= 40.0f))
		{
			nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_FALLATTACK);
			nowCost_ = NONE_COST;
		}
		//地面などに当たった場合
		else if (actor_.lock()->IsCollGroundFlag())
		{
			//ジャンプのキャンセル
			//ジャンプというより上下方向に働くベクトル
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);
			jumpFlag_ = false;

			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionComandFallAttack() 
{
	if (actor_.lock()->GetState() != ActorBase::STATE::FALL_ATTACK)
	{
		//状態を落下攻撃にする
		actor_.lock()->ChangeState(ActorBase::STATE::FALL_ATTACK);
		SetTime(TIME::ATTACK_RAG, 0.8f);
		//スタミナを減らす
		actor_.lock()->userInterface_->SubStamina(10.0f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
		if (actor_.lock()->animController_->GetStepAnimTime() >= 35.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			SphereF sph = actor_.lock()->Model2Sphere(250);
			if (actor_.lock()->collider_->CollisionSphere2Cap(sph))
			{
				//当たり判定フラグ
				colliderFlag_ = true;
				float power = actor_.lock()->GetAttackPower();
				actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
				actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() / 1.3f ,0.0f });
				actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 100.0f));
				actor_.lock()->camera_.lock()->SetPos(
					VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetBack(), 100.0f)));
				actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);

				actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);

				SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
			}
		}


		//落下攻撃処理
		if (!comandFlag_)
		{
			comandFlag_ = true;
			actor_.lock()->SetJumpVector(VGet(0.0f, actor_.lock()->GetJumpPower() / 2.0f, 0.0f));
		}
		//地面などに当たった場合
		else if (actor_.lock()->IsCollGroundFlag())
		{
			comandFlag_ = false;
			//ジャンプのキャンセル
			//ジャンプというより上下方向に働くベクトル
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);;
			//ジャンプの派生で落下攻撃を行うため
			//ジャンプフラグもオフにする
			jumpFlag_ = false;

			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}

			//地面に着いた後に、当たり判定用のエフェクトを発生させる
			if (colliderFlag_)
			{
				Position3DF effPos = VAdd(actor_.lock()->transform_->pos, VGet(0.0f, 50.0f, 0.0f));
				Rot3DF effRot = MyUtility::VECTOR_ZERO;
				actor_.lock()->PlayEffect(EffectManager::EFF::FALL_ATTACK, ActorBase::USE_EFFECT::FALL_ATTACK, effPos, effRot);
			}

			//瓦礫・風エフェクトの発生
			Position3DF effPos = VAdd(actor_.lock()->transform_->pos, VGet(0.0f, 50.0f, 0.0f));
			Rot3DF effRot = MyUtility::VECTOR_ZERO;
			effRot = Quaternion::ToEuler(
				Quaternion::AngleAxis(MyUtility::Deg2RadD(-50.0), MyUtility::AXIS_X));
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
		}
	}
}

void ActorAi::ActionComandDefence() 
{
	if (actor_.lock()->GetState() != ActorBase::STATE::DEFENCE)
	{
		//状態を防御にする
		actor_.lock()->ChangeState(ActorBase::STATE::DEFENCE);
		SetTime(TIME::JUST_RECEP_TIME, 0.02f *
			static_cast<float>(actor_.lock()->userInterface_->GetStateAvoidLevel()));

		//エフェクト
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		actor_.lock()->PlayEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA, effPos, effRot);
		//防御力
		actor_.lock()->userInterface_->AddDefence(actor_.lock()->GetStateDefenceMax());
		
		//自動で相手の方に向く
		FactEnemy();
	}
	else
	{
		//エフェクト
		Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
		Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
		actor_.lock()->SetEffectPos(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA, effPos);
		actor_.lock()->SetEffectRot(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA, effRot);
		actor_.lock()->SetWeight(1.7f);
		comandFlag_ = true;

		//防御し続ける限り魔力回復
		actor_.lock()->userInterface_->AddMagicPower(0.4f);
		//防御し続ける限りスタミナ消費
		actor_.lock()->userInterface_->SubStamina(0.3f);

		//ガードブレイク
		if (actor_.lock()->userInterface_->GetStateStamina() <= 0.0f &&
			!actor_.lock()->IsDamageAnimFlag())
		{
			//再びガードできるまでのラグ
			SetTime(TIME::GURD_RAG, 1.0f);

			actor_.lock()->SetDefaultWeight();
			actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(50.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::GURD_BREAK, ActorBase::USE_EFFECT::GURD_BREAK, effPos, effRot);

			actor_.lock()->userInterface_->SetDefence(0.0f);
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
			actor_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageGurdBreak(); });
			actor_.lock()->SoundPlay(SoundManager::SOD::GURD_BREAK, DX_PLAYTYPE_BACK);
		}

		if (mode_ == MODE::MANUAL)
		{
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_DEFENCE))]->IsCondition() == NONE_INDEX)
			{
				actor_.lock()->SetDefaultWeight();
				actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
				actor_.lock()->userInterface_->SetDefence(0.0f);
				comandFlag_ = false;
				//アクションを終了
				int nSearch = SearchNodeIdx(nowIndex_);
				if (nSearch >= 0)
				{
					EndNodeAction(nSearch);
				}
			}
			else if (ConditionComandAvoid() != NONE_INDEX)
			{
				actor_.lock()->SetDefaultWeight();
				actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
				actor_.lock()->userInterface_->SetDefence(0.0f);
				comandFlag_ = false;
				//アクションを遷移
				nowIndex_ = INT_INDEX(INDEX_ACT_COMAND_AVOID);
				nowCost_ = NONE_COST;
			}
		}
		else
		{
			if (actor_.lock()->animController_->GetStepAnimTime() >= 50.0f)
			{
				actor_.lock()->SetDefaultWeight();
				actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
				actor_.lock()->userInterface_->SetDefence(0.0f);
				comandFlag_ = false;
				//アクションを終了
				int nSearch = SearchNodeIdx(nowIndex_);
				if (nSearch >= 0)
				{
					EndNodeAction(nSearch);
				}
			}
		}

		//ジャスト判定
		if (timeCounter_.at(TIME::JUST_RECEP_TIME) > 0.0f && damageFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			damageFlag_ = false;
			SetTime(TIME::JUST_CHANCE_TIME, 1.5f);
			actor_.lock()->actorCam_->ChangeMode(Camera::MODE::JUST);
			actor_.lock()->enemy_.lock()->SetStopFrame(1.5f);
			actor_.lock()->userInterface_->SetDamagePower(0.0f);
			actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::JUST);
			actor_.lock()->SetJumpVector(MyUtility::VECTOR_ZERO);
			actor_.lock()->SetSwayVector(MyUtility::VECTOR_ZERO);

			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::JUST, ActorBase::USE_EFFECT::JUST, effPos, effRot);

			actor_.lock()->SoundPlay(SoundManager::SOD::JUST, DX_PLAYTYPE_BACK);

			//アクションの終了処理
			actor_.lock()->SetDefaultWeight();
			actor_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
			actor_.lock()->userInterface_->SetDefence(0.0f);

			//魔力回復
			actor_.lock()->userInterface_->AddMagicPower(20.0f);

			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
			//カウンターアタック条件に遷移
			if (mode_ == MODE::MANUAL)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_COUNTERATTACK);
			}
			else
			{
				nowIndex_ = INT_INDEX(INDEX_ACT_AUTO_COUNTER);
				SetTime(TIME::COUNTER_RAG, 0.3f);
			}
			nowCost_ = NONE_COST;
		}
		//防御状態でダメージを受けたとき、防御力経験値アップ
		if (damageFlag_)
		{
			damageFlag_ = false;
			actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);
			//スタミナが減る
			actor_.lock()->userInterface_->SubStamina(5.0f);
		}
	}
}

void ActorAi::ActionComandUnleashSkill() 
{
	
}

void ActorAi::ActionComandCounterAttack() 
{
	//常に相手の方を向く
	//自動で相手の方を向く
	FactEnemy();

	if (actor_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK)
	{
		//状態を弱攻撃_1にする
		actor_.lock()->ChangeState(ActorBase::STATE::COUNTER_ATTACK);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.5f);

		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
		if (actor_.lock()->animController_->GetStepAnimTime() >= 10.0f && !colliderFlag_)
		{
			VERTEX3D vertex[3];

			for (int ii = 1; ii < COUNTER_ATTACK_AREA; ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//当たり判定フラグ
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() / 2.3f ,0.0f });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 150.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetUp(), 180.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
					}
					//相手がカウントしていた連続攻撃回数をリセット
					actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();

					//エフェクト
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
					Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
					actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG, effPos, effRot);

					effPos = actor_.lock()->GetPlayerForwardPos(50.0f);
					actor_.lock()->PlayEffect(EffectManager::EFF::BLUR, ActorBase::USE_EFFECT::BLUR, effPos, effRot);

					//エフェクト
					actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG_2, effPos, effRot);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}


		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			Position3DF lockonPos = actor_.lock()->enemy_.lock()->transform_->pos;
			Dir3DF dir = VSub(actor_.lock()->transform_->pos,
				actor_.lock()->enemy_.lock()->transform_->pos);
			dir = VNorm(dir);
			lockonPos = VAdd(lockonPos, VScale(dir, 150.0f));
			actor_.lock()->transform_->pos = lockonPos;

			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(10.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::JUST, ActorBase::USE_EFFECT::JUST, effPos, effRot);
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			actor_.lock()->actorCam_->ChangeDefaultMode();
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void ActorAi::ActionComandFailCounterAttack()
{
	//アクションを終了
	int nSearch = SearchNodeIdx(nowIndex_);
	if (nSearch >= 0)
	{
		EndNodeAction(nSearch);
	}
}