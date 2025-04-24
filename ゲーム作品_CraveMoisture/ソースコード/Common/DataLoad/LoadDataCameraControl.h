#pragma once
#include "LoadData.h"

class DataCameraControl;
class DataPaths;

class LoadDataCameraControl
{
public:

	//データのロード
	static DataCameraControl LoadedDataCameraControl(const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};