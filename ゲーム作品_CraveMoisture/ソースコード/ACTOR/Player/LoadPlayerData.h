#pragma once
#include "../../Common/DataLoad/LoadData.h"

class PlayerData;
class DataPaths;

class LoadPlayerData
{
public:

	// プレイヤー制御データのロード
	static PlayerData LoadedPlayerData(
		const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};