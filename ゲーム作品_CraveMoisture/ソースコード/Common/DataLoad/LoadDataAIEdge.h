#pragma once
#include "LoadData.h"

class DataAIEdge;
class DataPaths;

class LoadDataAIEdge
{
public:

	//�f�[�^�̃��[�h
	static DataAIEdge LoadedDataAIEdge(const DataPaths& dataPath, const LoadData::LoadDataInfo& info);

};