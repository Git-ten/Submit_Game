#include "../../Common/DataLoad/DataAIEdge.h"
#include "../../Common/DataLoad/LoadDataAIEdge.h"
#include "../../Application/Application.h"
#include "EnemyAIBase.h"

EnemyAIBase::EnemyAIBase(EnemyState::STATE& state, const DIR_BACK& dirBack,
	const EnemyData::RagTimesData& ragTimes):
	nowState_(state),
	dirBack_(dirBack)
{
	InitRagTime(ragTimes);
}

EnemyAIBase::~EnemyAIBase(void)
{
}

void EnemyAIBase::SetUpIsDistance(Position3DF& enemyPos, Position3DF& playerPos)
{
	distance_ = std::make_unique<IsDistance>(enemyPos, playerPos);
}

const bool EnemyAIBase::IsDistanceNull(void)
{
	return distance_ == nullptr;
}

void EnemyAIBase::SetUpRagTime(const RAG_NAME& ragTimeName)
{
	const auto& iteTime = ragTimes_.time.find(ragTimeName);
	const auto& iteTimeMax = ragTimes_.time_Max.find(ragTimeName);
	assert(iteTime != ragTimes_.time.end() || !"ÇªÇÃRAG_TIME::timeÇÕê›íËÇ≥ÇÍÇƒÇ¢Ç‹ÇπÇÒ");
	assert(iteTimeMax != ragTimes_.time_Max.end() || !"ÇªÇÃRAG_TIME::time_MaxÇÕê›íËÇ≥ÇÍÇƒÇ¢Ç‹ÇπÇÒ");

	ragTimes_.time.at(ragTimeName).InitTotalNowTime();
}

void EnemyAIBase::UpdateSpecial(void)
{
	UpdateRagTime();
}

void EnemyAIBase::InitRagTime(const EnemyData::RagTimesData& ragTimes)
{
	ragTimes_.time.emplace(RAG_NAME::ATTACK_1_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_1_RAG_TIME, ragTimes.attackRagTime_1);
	ragTimes_.time.at(RAG_NAME::ATTACK_1_RAG_TIME).SetTotalNowTime(ragTimes.attackRagTime_1);

	ragTimes_.time.emplace(RAG_NAME::ATTACK_2_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_2_RAG_TIME, ragTimes.attackRagTime_2);
	ragTimes_.time.at(RAG_NAME::ATTACK_2_RAG_TIME).SetTotalNowTime(ragTimes.attackRagTime_2);

	ragTimes_.time.emplace(RAG_NAME::ATTACK_3_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_3_RAG_TIME, ragTimes.attackRagTime_3);
	ragTimes_.time.at(RAG_NAME::ATTACK_3_RAG_TIME).SetTotalNowTime(ragTimes.attackRagTime_3);

	ragTimes_.time.emplace(RAG_NAME::ATTACK_4_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_4_RAG_TIME, ragTimes.attackRagTime_4);
	ragTimes_.time.at(RAG_NAME::ATTACK_4_RAG_TIME).SetTotalNowTime(ragTimes.attackRagTime_4);

	ragTimes_.time.emplace(RAG_NAME::ATTACK_5_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_5_RAG_TIME, ragTimes.attackRagTime_5);
	ragTimes_.time.at(RAG_NAME::ATTACK_5_RAG_TIME).SetTotalNowTime(ragTimes.attackRagTime_5);

	ragTimes_.time.emplace(RAG_NAME::ATTACK_6_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_6_RAG_TIME, ragTimes.attackRagTime_6);
	ragTimes_.time.at(RAG_NAME::ATTACK_6_RAG_TIME).SetTotalNowTime(ragTimes.attackRagTime_6);
	
	ragTimes_.time.emplace(RAG_NAME::ATTACK_AWAKE_RAG_TIME, Time());
	ragTimes_.time_Max.emplace(RAG_NAME::ATTACK_AWAKE_RAG_TIME, ragTimes.attackAwakeRagTime);
	ragTimes_.time.at(RAG_NAME::ATTACK_AWAKE_RAG_TIME).SetTotalNowTime(ragTimes.attackAwakeRagTime);
}

void EnemyAIBase::UpdateRagTime(void)
{
	for (auto& time : ragTimes_.time)
	{
		if (time.second.GetTotalNowTime() < ragTimes_.time_Max.at(time.first))
		{
			time.second.UpdateTime();
		}
	}
}

void EnemyAIBase::CreateNode(void)
{
	AddNode(static_cast<int>(NODE_NAME::PARENT_NODE), [&]() {ChangeEnemyStateEnd(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_WOLK_NODE), [&]() {ChangeEnemyStateWalk(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_1_NODE), [&]() {ChangeEnemyStateAction_1(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_2_NODE), [&]() {ChangeEnemyStateAction_2(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_3_NODE), [&]() {ChangeEnemyStateAction_3(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_4_NODE), [&]() {ChangeEnemyStateAction_4(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_5_NODE), [&]() {ChangeEnemyStateAction_5(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_6_NODE), [&]() {ChangeEnemyStateAction_6(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_ATTACK_AWAKE_NODE), [&]() {ChangeEnemyStateAction_Awake(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_END_SHORT_NODE), [&]() {ChangeEnemyStateActionEndShort(); });
	AddNode(static_cast<int>(NODE_NAME::ACTION_END_LONG_NODE), [&]() {ChangeEnemyStateActionEndLong(); });
}

void EnemyAIBase::ConnectEdge(void)
{
	SetUpNodeNameStrings();
	SetUpConditionNameString();

	LoadData::LoadDataInfo info = { "Enemy_AIInfo.json", "TransToActionEdge" };
	DataAIEdge data = DataAIEdge();

	std::function<void(void)>LOAD =
		[&]()
		{
			data = LoadDataAIEdge::LoadedDataAIEdge(
				Application::GetInstance().GetDataPaths(), info);
			AddEdge(info.dataName,
				static_cast<int>(NodeNameStringToNODE_NAME(data.fromIndexName)),
				NodeNameStringToNODE_NAMEVec(data.endIndexNames),
				ConditionNameStringToConditionVec(data.conditionNames),
				data.costs);
		};


	LOAD();
	info.dataName = "TransToAttack_1ContinueEdge";
	LOAD();
	info.dataName = "TransToAttack_2ContinueEdge";
	LOAD();
	info.dataName = "TransToAttack_3ContinueEdge";
	LOAD();
	info.dataName = "TransToAttack_4ContinueEdge";
	LOAD();
	info.dataName = "TransToAttack_5ContinueEdge";
	LOAD();
	info.dataName = "TransToAttack_6ContinueEdge";
	LOAD();

	node_NameStrings_.clear();
	condition_NameStrings_.clear();
}

void EnemyAIBase::SetUpNodeNameStrings(void)
{
	node_NameStrings_.clear();

	node_NameStrings_.emplace("PARENT_NODE", NODE_NAME::PARENT_NODE);
	node_NameStrings_.emplace("ACTION_MOVE_NODE", NODE_NAME::ACTION_WOLK_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_1_NODE", NODE_NAME::ACTION_ATTACK_1_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_2_NODE", NODE_NAME::ACTION_ATTACK_2_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_3_NODE", NODE_NAME::ACTION_ATTACK_3_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_4_NODE", NODE_NAME::ACTION_ATTACK_4_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_5_NODE", NODE_NAME::ACTION_ATTACK_5_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_6_NODE", NODE_NAME::ACTION_ATTACK_6_NODE);
	node_NameStrings_.emplace("ACTION_ATTACK_AWAKE_NODE", NODE_NAME::ACTION_ATTACK_AWAKE_NODE);
	node_NameStrings_.emplace("ACTION_END_SHORT_NODE", NODE_NAME::ACTION_END_SHORT_NODE);
	node_NameStrings_.emplace("ACTION_END_LONG_NODE", NODE_NAME::ACTION_END_LONG_NODE);
}

const EnemyAIBase::NODE_NAME& EnemyAIBase::NodeNameStringToNODE_NAME(const std::string& name)
{
	const auto& ite = node_NameStrings_.find(name);
	assert(ite != node_NameStrings_.end() || !"EnemyAIBase::ë∂ç›ÇµÇ»Ç¢ÉmÅ[ÉhÇÃñºëOÇ≈Ç∑");

	return node_NameStrings_.at(name);
}

const std::vector<int> EnemyAIBase::NodeNameStringToNODE_NAMEVec(const NAMES& names)
{
	std::vector<int>nodeVec;

	for (const auto& name : names)
	{
		nodeVec.push_back(static_cast<int>(NodeNameStringToNODE_NAME(name)));
	}

	return nodeVec;
}

void EnemyAIBase::SetUpConditionNameString(void)
{
	condition_NameStrings_.clear();
	condition_NameStrings_.emplace("ConditionMove", [&]() {return ConditionWalk(); });
	condition_NameStrings_.emplace("ConditionAttack_1", [&]() {return ConditionAttack_1(); });
	condition_NameStrings_.emplace("ConditionAttack_2", [&]() {return ConditionAttack_2(); });
	condition_NameStrings_.emplace("ConditionAttack_3", [&]() {return ConditionAttack_3(); });
	condition_NameStrings_.emplace("ConditionAttack_4", [&]() {return ConditionAttack_4(); });
	condition_NameStrings_.emplace("ConditionAttack_5", [&]() {return ConditionAttack_5(); });
	condition_NameStrings_.emplace("ConditionAttack_6", [&]() {return ConditionAttack_6(); });
	condition_NameStrings_.emplace("ConditionAttack_Awake", [&]() {return ConditionAttack_Awake(); });
	condition_NameStrings_.emplace("ConditionEndAttack_1", [&]() {return ConditionEndAttack_1_Rag(); });
	condition_NameStrings_.emplace("ConditionEndAttack_2", [&]() {return ConditionEndAttack_2_Rag(); });
	condition_NameStrings_.emplace("ConditionEndAttack_3", [&]() {return ConditionEndAttack_3_Rag(); });
	condition_NameStrings_.emplace("ConditionEndAttack_4", [&]() {return ConditionEndAttack_4_Rag(); });
	condition_NameStrings_.emplace("ConditionEndAttack_5", [&]() {return ConditionEndAttack_5_Rag(); });
	condition_NameStrings_.emplace("ConditionEndAttack_6", [&]() {return ConditionEndAttack_6_Rag(); });
	condition_NameStrings_.emplace("ConditionNearDistance", [&]() {return ConditionNearDistance(); });
	condition_NameStrings_.emplace("ConditionNearDisBack", [&]() {return ConditionNearDisBack(); });
	condition_NameStrings_.emplace("ConditionMediumDistance", [&]() {return ConditionMediumDistance(); });
}

const EnemyAIBase::CONDITION& EnemyAIBase::ConditionNameStringToCondition(const std::string& name)
{
	const auto& ite = condition_NameStrings_.find(name);
	assert(ite != condition_NameStrings_.end() || !"EnemyAIBase::ë∂ç›ÇµÇ»Ç¢ëJà⁄èåèÇÃñºëOÇ≈Ç∑");

	return condition_NameStrings_.at(name);
}

const std::vector<EnemyAIBase::CONDITION>
EnemyAIBase::ConditionNameStringToConditionVec(const NAMES& names)
{
	std::vector<CONDITION>conditionVec;

	for (const auto& name : names)
	{
		conditionVec.push_back(ConditionNameStringToCondition(name));
	}

	return conditionVec;
}

void EnemyAIBase::ChangeEnemyStateEnd(void)
{
	EndAIAction();
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) | EnemyState::IDLE);
}

void EnemyAIBase::ChangeEnemyStateWalk(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) | EnemyState::WALK);
}

void EnemyAIBase::ChangeEnemyStateAction_1(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) | 
		EnemyState::ATTACK | EnemyState::ATTACK_1);
}

void EnemyAIBase::ChangeEnemyStateAction_2(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) |
		EnemyState::ATTACK | EnemyState::ATTACK_2);
}

void EnemyAIBase::ChangeEnemyStateAction_3(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) |
		EnemyState::ATTACK | EnemyState::ATTACK_3);
}

void EnemyAIBase::ChangeEnemyStateAction_4(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) |
		EnemyState::ATTACK | EnemyState::ATTACK_4);
}

void EnemyAIBase::ChangeEnemyStateAction_5(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) |
		EnemyState::ATTACK | EnemyState::ATTACK_5);
}

void EnemyAIBase::ChangeEnemyStateAction_6(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) |
		EnemyState::ATTACK | EnemyState::ATTACK_6);
}

void EnemyAIBase::ChangeEnemyStateAction_Awake(void)
{
	nowState_ = static_cast<EnemyState::STATE>((EnemyState::AWAKE & nowState_) |
		EnemyState::ATTACK | EnemyState::ATTACK_AWAKE);
}

void EnemyAIBase::ChangeEnemyStateActionEndShort(void)
{
	ChangeEnemyStateEnd();

	//AIÇíZéûä‘évçlí‚é~
	LockThinkShortTime();
}

void EnemyAIBase::ChangeEnemyStateActionEndLong(void)
{
	ChangeEnemyStateEnd();

	//AIÇí∑éûä‘évçlí‚é~
	LockThinkLongTime();
}

const bool EnemyAIBase::ConditionWalk(void)
{
	const float dis = distance_->DistanceActors();

	return !IsAttack() && 
			dis > IsDistance::CONDITION_NEAR_DISTANCE;
}

const bool EnemyAIBase::ConditionAttack_1(void)
{
	const float dis = distance_->DistanceActors();

	return !ConditionEndAttack_1_Rag() &&
			dis < IsDistance::CONDITION_NEAR_DISTANCE;
}

const bool EnemyAIBase::ConditionAttack_2(void)
{
	const float dis = distance_->DistanceActors();

	return !ConditionEndAttack_2_Rag() &&
		dis < IsDistance::CONDITION_NEAR_DISTANCE;
}

const bool EnemyAIBase::ConditionAttack_3(void)
{
	const float dis = distance_->DistanceActors();

	return !ConditionEndAttack_3_Rag() &&
		IsDistance::CONDITION_NEAR_DISTANCE < dis &&
		dis <= IsDistance::CONDITION_MEDIUM_DISTANCE;
}

const bool EnemyAIBase::ConditionAttack_4(void)
{
	const float dis = distance_->DistanceActors();

	return !ConditionEndAttack_4_Rag() &&
		dis < IsDistance::CONDITION_NEAR_DISTANCE;
}

const bool EnemyAIBase::ConditionAttack_5(void)
{
	const float dis = distance_->DistanceActors();

	return !ConditionEndAttack_5_Rag() &&
		((IsDistance::CONDITION_MEDIUM_DISTANCE < dis &&
			dis < IsDistance::CONDITION_FAR_DISTANCE) ||
		IsDistance::CONDITION_MOST_NEAR_DISTANCE > dis);
}

const bool EnemyAIBase::ConditionAttack_6(void)
{
	const float dis = distance_->DistanceActors();

	const Dir3DF& dir = distance_->DirShareActorToActor();

	return !ConditionEndAttack_6_Rag() &&
			dis <= IsDistance::CONDITION_NEAR_DISTANCE &&
			VDot(dir, dirBack_()) > 0.0f;
}

const bool EnemyAIBase::ConditionAttack_Awake(void)
{
	return !ConditionEndAttack_Awake_Rag() &&
			(nowState_ & EnemyState::AWAKE);
}

const bool EnemyAIBase::ConditionNearDistance(void)
{
	return distance_->IsNearDistance() && IsEndAIAction();
}

const bool EnemyAIBase::ConditionNearDisBack(void)
{
	const Dir3DF& dir = distance_->DirShareActorToActor();

	return distance_->IsNearDistance() &&
			VDot(dir, dirBack_()) > 0.0f &&
			IsEndAIAction();
}

const bool EnemyAIBase::ConditionMediumDistance(void)
{
	return distance_->IsMediumDistance() && IsEndAIAction();
}

const bool EnemyAIBase::ConditionFarDistance(void)
{
	return distance_->IsFarDistance() && IsEndAIAction();
}

const bool EnemyAIBase::ConditionEndAttack_1_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_1_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_1_RAG_TIME);
}

const bool EnemyAIBase::ConditionEndAttack_2_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_2_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_2_RAG_TIME);
}

const bool EnemyAIBase::ConditionEndAttack_3_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_3_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_3_RAG_TIME);
}

const bool EnemyAIBase::ConditionEndAttack_4_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_4_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_4_RAG_TIME);
}

const bool EnemyAIBase::ConditionEndAttack_5_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_5_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_5_RAG_TIME);
}

const bool EnemyAIBase::ConditionEndAttack_6_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_6_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_6_RAG_TIME);
}

const bool EnemyAIBase::ConditionEndAttack_Awake_Rag(void)const
{
	return ragTimes_.time.at(RAG_NAME::ATTACK_AWAKE_RAG_TIME).GetTotalNowTime() <
		ragTimes_.time_Max.at(RAG_NAME::ATTACK_AWAKE_RAG_TIME);
}

const bool EnemyAIBase::IsAttack(void)const
{
	return  nowState_ & EnemyState::ATTACK_1 ||
			nowState_ & EnemyState::ATTACK_2 ||
			nowState_ & EnemyState::ATTACK_3 ||
			nowState_ & EnemyState::ATTACK_4 ||
			nowState_ & EnemyState::ATTACK_5 ||
			nowState_ & EnemyState::ATTACK_6 ||
			nowState_ & EnemyState::ATTACK_AWAKE;
}