#pragma once
#include "../../Common/DataLoad/LoadData.h"

class EnemyData;
class DataPaths;

class LoadEnemyData
{
public:

	// プレイヤー制御データのロード
	static EnemyData LoadedEnemyData(
		const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};