#pragma once
#include "LoadData.h"
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"
#include "../../Application/Singleton_Manager/EffectResourceManager/EffectResourceManager.h"

class DataImageDiv;
class DataPaths;

class LoadDataImageDiv
{
public:

	//データのロード
	static DataImageDiv LoadedDataImageDiv(const DataPaths& dataPath, const ResourceManager::RESOURCE_ID& id,
		const LoadData::LoadDataInfo& info);

	//データのロード
	static DataImageDiv LoadedDataImageDiv(const DataPaths& dataPath, const EffectResourceManager::EFFECT_RESOURCE_ID& id,
		const LoadData::LoadDataInfo& info);

private:

	// ローディング処理
	static void LoadDataImageInfo(const DataPaths& dataPath, DataImageDiv& imageDiv, const LoadData::LoadDataInfo& info);

};