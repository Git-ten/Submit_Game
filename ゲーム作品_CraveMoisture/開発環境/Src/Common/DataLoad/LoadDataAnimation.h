#pragma once
#include "LoadData.h"

class DataAnimation;
class DataPaths;

class LoadDataAnimation
{
public:

	//データのロード
	static DataAnimation LoadedDataAnimation(const DataPaths& dataPath,	const LoadData::LoadDataInfo& info);
};