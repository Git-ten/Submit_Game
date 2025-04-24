#include "../../Common/DataLoad/DataAIEdge.h"
#include "../../Common/DataLoad/LoadDataAIEdge.h"
#include "../../Application/Application.h"
#include "TutorialBearAI.h"

TutorialBearAI::TutorialBearAI(EnemyState::STATE& state, const DIR_BACK& dirBack,
	const EnemyData::RagTimesData& ragTimes):
	EnemyAIBase(state, dirBack, ragTimes)
{
}

TutorialBearAI::~TutorialBearAI(void)
{
}

void TutorialBearAI::ConnectEdge(void)
{
	SetUpNodeNameStrings();
	SetUpConditionNameString();

	LoadData::LoadDataInfo info = { "Bear_AIInfo.json", "TransToActionEdge" };
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

	node_NameStrings_.clear();
	condition_NameStrings_.clear();
}