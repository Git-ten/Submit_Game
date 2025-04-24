#pragma once
#include "LoadData.h"
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"

class DataModel;
class DataPaths;

class LoadDataModel
{
public:

	//データのロード
	static DataModel LoadedDataModel(const DataPaths& dataPath, const ResourceManager::RESOURCE_ID& id,
		const LoadData::LoadDataInfo& info);
};