#include <string>
#include "DataPaths.h"
#include "DataAIEdge.h"
#include "LoadDataAIEdge.h"

DataAIEdge LoadDataAIEdge::LoadedDataAIEdge(const DataPaths& dataPath, const LoadData::LoadDataInfo& info)
{
	LoadData loadData;
	DataAIEdge edgeData;

	// ƒpƒX
	const std::string EDGE_DATA_NAME = dataPath.PATH_JSON + info.fileName;

	const std::string EDGE_FROM_INDEX_NAME = "from";
	const std::string EDGE_END_INDEX_NAME = "end";
	const std::string EDGE_CONDITION_NAME = "condition";
	const std::string EDGE_COST_NAME = "cost";

	loadData.LoadStart(EDGE_DATA_NAME);

	loadData.LoadedString(info.dataName, EDGE_FROM_INDEX_NAME, edgeData.fromIndexName);
	loadData.LoadedStringVector(info.dataName, EDGE_END_INDEX_NAME, edgeData.endIndexNames);
	loadData.LoadedStringVector(info.dataName, EDGE_CONDITION_NAME, edgeData.conditionNames);
	loadData.LoadedFloatVector(info.dataName, EDGE_COST_NAME, edgeData.costs);

	loadData.LoadEnd();

	return edgeData;
}
