#pragma once
#include "../../Common/DataLoad/LoadData.h"

class PlayerData;
class DataPaths;

class LoadPlayerData
{
public:

	// �v���C���[����f�[�^�̃��[�h
	static PlayerData LoadedPlayerData(
		const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};