#pragma once
#include "LoadData.h"

class DataEffect;
class DataPaths;

class LoadDataEffect
{
public:

	// �f�[�^�̃��[�h
	static DataEffect LoadedDataEffect(const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};