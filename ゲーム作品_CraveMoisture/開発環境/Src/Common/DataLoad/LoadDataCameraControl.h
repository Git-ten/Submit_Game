#pragma once
#include "LoadData.h"

class DataCameraControl;
class DataPaths;

class LoadDataCameraControl
{
public:

	//�f�[�^�̃��[�h
	static DataCameraControl LoadedDataCameraControl(const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};