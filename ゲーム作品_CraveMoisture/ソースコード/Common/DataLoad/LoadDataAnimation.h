#pragma once
#include "LoadData.h"

class DataAnimation;
class DataPaths;

class LoadDataAnimation
{
public:

	//�f�[�^�̃��[�h
	static DataAnimation LoadedDataAnimation(const DataPaths& dataPath,	const LoadData::LoadDataInfo& info);
};