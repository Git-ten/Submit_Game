#include"../../ActorBase.h"
#include"../../Common/UserInterface.h"
#include"../../Common/AnimationController.h"
#include"Node/NodeBase.h"
#include"Node/ActionNode.h"
#include"Node/ConditionNode.h"
#include"Node/Edge.h"
#include"ActorAi.h"
#include "NpcAi.h"
#include<cmath>

NpcAi::NpcAi(std::weak_ptr<ActorAi>targetAi, std::weak_ptr<ActorBase>actor)
{
	targetAi_ = targetAi;
	actor_ = actor;
}

NpcAi::~NpcAi()
{
}

void NpcAi::Update()
{
	//é¿çsâ¬î\Ç»ÉmÅ[ÉhÇÃíTçı
	int sCounter = 0;
	while (sCounter < MAX_NODE_SEARCH)
	{
		nowIndex_ = ConditionBranch();
		
		int nSearch = SearchNodeIdx(nowIndex_);
		if (nSearch >= 0)
		{
			//èIí[ÉmÅ[ÉhÇÃèÍçáÇÃÇ›èàóù
			if (nodes_[nSearch]->IsEndNode())
			{
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

void NpcAi::DebugDraw()
{
}

void NpcAi::CreateNode()
{
	AddConditionNode(INT_INDEX(INDEX_CON_OPERATION_MOVE), [&]() {return ConditionOperateMove(); });
	AddConditionNode(INT_INDEX(INDEX_CON_OPERATION_ESCAPE), [&]() {return ConditionOperateEscape(); });
	AddConditionNode(INT_INDEX(INDEX_CON_OPERATION_ATTACK), [&]() {return ConditionOperateAttack(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE), [&]() {return ConditionMove(); });
	AddConditionNode(INT_INDEX(INDEX_CON_MOVE_BRANCH), [&]() {return ConditionMoveBranch(); });
	AddConditionNode(INT_INDEX(INDEX_CON_ESCAPE), [&]() {return ConditionEscape(); });
	AddConditionNode(INT_INDEX(INDEX_CON_ESCAPE_BRANCH), [&]() {return ConditionEscapeBranch(); });
	AddConditionNode(INT_INDEX(INDEX_CON_ATTACK), [&]() {return ConditionAttack(); });
	AddConditionNode(INT_INDEX(INDEX_CON_ATTACK_BRANCH), [&]() {return ConditionAttackBranch(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_FAR), [&]() {ActionMoveFar(); });
	AddActionNode(INT_INDEX(INDEX_ACT_MOVE_MEDIUM), [&]() {ActionMoveMedium(); });
	AddActionNode(INT_INDEX(INDEX_ACT_ESCAPE_AVOID), [&]() {ActionEscapeAvoid(); });
	AddActionNode(INT_INDEX(INDEX_ACT_ESCAPE_RUN), [&]() {ActionEscapeRun(); });
	AddActionNode(INT_INDEX(INDEX_ACT_ESCAPE_DEFENCE), [&]() {ActionEscapeDefence(); });
	AddActionNode(INT_INDEX(INDEX_ACT_ATTACK_WEEK), [&]() {ActionAttackWeek(); });
	AddActionNode(INT_INDEX(INDEX_ACT_ATTACK_STRONG), [&]() {ActionAttackStrong(); });
	AddActionNode(INT_INDEX(INDEX_ACT_UNLEASHSKILL), [&]() {ActionUnleashSkill(); });
}

void NpcAi::ConnectNode()
{
	//çÏêÌÅFìGÇ…ãﬂÇ√Ç≠
	int from = INT_INDEX(INDEX_CON_OPERATION_MOVE);
	int to = INT_INDEX(INDEX_CON_MOVE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.12f);

	//çÏêÌÅFìGÇ©ÇÁì¶Ç∞ÇÈ
	from = INT_INDEX(INDEX_CON_OPERATION_ESCAPE);
	to = INT_INDEX(INDEX_CON_ESCAPE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.13f);

	//çÏêÌÅFìGÇçUåÇ
	from = INT_INDEX(INDEX_CON_OPERATION_ATTACK);
	to = INT_INDEX(INDEX_CON_ATTACK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.125f);



	//à⁄ìÆèåèÅ[ÅÑà⁄ìÆèåèï™äÚ
	from = INT_INDEX(INDEX_CON_MOVE);
	to = INT_INDEX(INDEX_CON_MOVE_BRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.15f);

	//à⁄ìÆèåèï™äÚÅ[ÅÑâìãóó£èàóù
	from = INT_INDEX(INDEX_CON_MOVE_BRANCH);
	to = INT_INDEX(INDEX_ACT_MOVE_FAR);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.3f);

	//à⁄ìÆèåèï™äÚÅ[ÅÑíÜãóó£èàóù
	from = INT_INDEX(INDEX_CON_MOVE_BRANCH);
	to = INT_INDEX(INDEX_ACT_MOVE_MEDIUM);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.2f);

	//âÒîèåèÅ[ÅÑâÒîèåèï™äÚ
	from = INT_INDEX(INDEX_CON_ESCAPE);
	to = INT_INDEX(INDEX_CON_ESCAPE_BRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.15f);

	//âÒîèåèï™äÚÅ[ÅÑÉAÉ{ÉCÉhèàóù
	from = INT_INDEX(INDEX_CON_ESCAPE_BRANCH);
	to = INT_INDEX(INDEX_ACT_ESCAPE_AVOID);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.2f);

	//âÒîèåèï™äÚÅ[ÅÑëñÇÈèàóù
	from = INT_INDEX(INDEX_CON_ESCAPE_BRANCH);
	to = INT_INDEX(INDEX_ACT_ESCAPE_RUN);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.15f);

	//âÒîèåèï™äÚÅ[ÅÑñhå‰èàóù
	from = INT_INDEX(INDEX_CON_ESCAPE_BRANCH);
	to = INT_INDEX(INDEX_ACT_ESCAPE_DEFENCE);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.2f);

	//çUåÇèåèÅ[ÅÑçUåÇèåèï™äÚ
	from = INT_INDEX(INDEX_CON_ATTACK);
	to = INT_INDEX(INDEX_CON_ATTACK_BRANCH);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.14f);

	//çUåÇèåèï™äÚÅ[ÅÑé„çUåÇèàóù
	from = INT_INDEX(INDEX_CON_ATTACK_BRANCH);
	to = INT_INDEX(INDEX_ACT_ATTACK_WEEK);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.2f);

	//çUåÇèåèï™äÚÅ[ÅÑé„ÅEã≠çUåÇèàóù
	from = INT_INDEX(INDEX_CON_ATTACK_BRANCH);
	to = INT_INDEX(INDEX_ACT_ATTACK_STRONG);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.3f);

	//çUåÇèåèï™äÚÅ[ÅÑïKéEãZèàóù
	from = INT_INDEX(INDEX_CON_ATTACK_BRANCH);
	to = INT_INDEX(INDEX_ACT_UNLEASHSKILL);
	AddConnectEdge(from, to, DEFAULT_NODE_COST + 0.4f);
}

int NpcAi::ConditionBranch()
{
	int ans = nowIndex_;

	if (nowIndex_ == NONE_INDEX)
	{
		ConditionStartNode(ans);
	}
	else
	{
		NodeTransition(nowIndex_, ans);
	}
	return ans;
}

void NpcAi::ConditionStartNode(int& idx)
{
	NodeTransition(INT_INDEX(INDEX_CON_OPERATION_ATTACK), idx);
	NodeTransition(INT_INDEX(INDEX_CON_OPERATION_MOVE), idx);
	NodeTransition(INT_INDEX(INDEX_CON_OPERATION_ESCAPE), idx);
}

int NpcAi::ConditionOperateMove()
{
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis >= std::powf(ActorAi::DISTANCE_MEDIUM, 2.0f) &&
		actor_.lock()->userInterface_->GetStateStamina() >=
		actor_.lock()->userInterface_->GetStateMaxStamina())
	{
		return INT_INDEX(INDEX_CON_MOVE);
	}

	return NONE_INDEX;
}

int NpcAi::ConditionOperateEscape()
{
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 2.0f) &&
		actor_.lock()->userInterface_->GetStateHp() <
		actor_.lock()->userInterface_->GetStateMaxHp() &&
		actor_.lock()->userInterface_->GetStateStamina() <
		actor_.lock()->userInterface_->GetStateMaxStamina() / 1.1f)
	{
		return INT_INDEX(INDEX_CON_ESCAPE);
	}

	return NONE_INDEX;
}

int NpcAi::ConditionOperateAttack()
{
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);

	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 2.7f) &&
		actor_.lock()->userInterface_->GetStateStamina() >
		actor_.lock()->userInterface_->GetStateMaxStamina() / 3.0f)
	{
		return INT_INDEX(INDEX_CON_ATTACK);
	}

	return NONE_INDEX;
}

int NpcAi::ConditionMove()
{
	return INT_INDEX(INDEX_CON_MOVE_BRANCH);
}

int NpcAi::ConditionMoveBranch()
{
	float dis = VSquareSize
		(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
		);

	if (dis >= std::powf(ActorAi::DISTANCE_FAR, 2.0f))
	{
		return INT_INDEX(INDEX_ACT_MOVE_FAR);
	}

	if (dis >= std::powf(ActorAi::DISTANCE_MEDIUM, 2.0f))
	{
		return INT_INDEX(INDEX_ACT_MOVE_MEDIUM);
	}

	return NONE_INDEX;
}

int NpcAi::ConditionEscape()
{
	return INT_INDEX(INDEX_CON_ESCAPE_BRANCH);
}

int NpcAi::ConditionEscapeBranch()
{
	float dis = VSquareSize
	(VSub(actor_.lock()->transform_->pos, actor_.lock()->enemy_.lock()->transform_->pos)
	);
	int ans = INT_INDEX(NONE_INDEX);
	if (dis >= std::powf(ActorAi::DISTANCE_MEDIUM, 1.1f))
	{
		if (actor_.lock()->userInterface_->GetStateHp() <=
			actor_.lock()->userInterface_->GetStateMaxHp() / 1.1f &&
			actor_.lock()->userInterface_->GetStateStamina() >=
			actor_.lock()->userInterface_->GetStateMaxStamina() / 2.0f)
		{
			ans = INT_INDEX(INDEX_ACT_ESCAPE_DEFENCE);
		}
	}
	else if (actor_.lock()->userInterface_->GetStateHp() <=
		actor_.lock()->userInterface_->GetStateMaxHp() / 2.0f &&
		actor_.lock()->userInterface_->GetStateStamina() >=
		actor_.lock()->userInterface_->GetStateMaxStamina() / 2.5f)
	{
		ans = INT_INDEX(INDEX_ACT_ESCAPE_AVOID);
	}
	else
	{
		ans = INT_INDEX(INDEX_ACT_ESCAPE_AVOID);
	}

	return ans;
}

int NpcAi::ConditionAttack()
{
	return INT_INDEX(INDEX_CON_ATTACK_BRANCH);
}

int NpcAi::ConditionAttackBranch()
{
	if (actor_.lock()->userInterface_->GetStateHp() < 
		actor_.lock()->userInterface_->GetStateMaxHp() / 3.0f && 
		actor_.lock()->IsUnleashSkill())
	{
		return INT_INDEX(INDEX_ACT_UNLEASHSKILL);
	}
	else if (actor_.lock()->userInterface_->GetStateStamina() >=
		actor_.lock()->userInterface_->GetStateMaxStamina() / 2.0f)
	{
		return INT_INDEX(INDEX_ACT_ATTACK_WEEK);
	}
	else
	{
		return INT_INDEX(INDEX_ACT_ATTACK_STRONG);
	}

	return NONE_INDEX;
}

void NpcAi::ActionMoveFar()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_QUICKMOVE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_MOVE_FAR &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK)
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_MOVE_FAR);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionMoveMedium()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_QUICKMOVE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_MOVE_MEDIUM &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK)
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_MOVE_MEDIUM);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionEscapeAvoid()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_AVOID &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK)
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_AVOID);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionEscapeRun()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_RUN &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK)
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_RUN);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionEscapeDefence()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK)
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_DEFENCE);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionAttackWeek()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_CON_COMAND_WEEKBRANCH &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_JUMP &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_FALLATTACK &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_WEEK_6 &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK &&
		!actor_.lock()->IsDamageAnimFlag() && (actor_.lock()->animController_->GetStepAnimTime() > actor_.lock()->GetAttackHappen() ||
		actor_.lock()->GetState() == ActorBase::STATE::IDLE))
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_WEEK);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionAttackStrong()
{
	if (targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_CON_COMAND_STRONGBRANCH &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_JUMP &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_FALLATTACK &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_WEEK_6 &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_STRONG_3 &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_DEFENCE &&
		targetAi_.lock()->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_AUTO_COUNTER &&
		actor_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX::INDEX_ACT_COMAND_COUNTERATTACK &&
		!actor_.lock()->IsDamageAnimFlag() &&
		(actor_.lock()->animController_->GetStepAnimTime() > 25.0f || actor_.lock()->GetState() == ActorBase::STATE::IDLE))
	{
		targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_CON_AUTO_STRONG);
		nowIndex_ = NONE_INDEX;
		nowCost_ = NONE_COST;
	}
}

void NpcAi::ActionUnleashSkill(void)
{
	targetAi_.lock()->SetNowIndex(ActorAi::INDEX::INDEX_ACT_COMAND_UNLEASHSKILL);
	nowIndex_ = NONE_INDEX;
	nowCost_ = NONE_COST;
	
}