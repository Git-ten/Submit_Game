#pragma once
#include "LoadData.h"
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"

class DataImage;
class DataPaths;

class LoadDataImage
{
public:

	//データのロード
	static DataImage LoadedDataImage(const DataPaths& dataPath, const ResourceManager::RESOURCE_ID& id,
		const LoadData::LoadDataInfo& info);

};