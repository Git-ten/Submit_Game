#pragma once
#include "../../Common/DataLoad/LoadData.h"

class EnemyData;
class DataPaths;

class LoadEnemyData
{
public:

	// �v���C���[����f�[�^�̃��[�h
	static EnemyData LoadedEnemyData(
		const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};