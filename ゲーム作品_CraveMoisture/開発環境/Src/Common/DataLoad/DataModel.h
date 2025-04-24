#pragma once
#include "../../Common/Quaternion.h"
#include "../../Utility/VectorHeader.h"

class DataModel
{
public:

	int modelId;				// モデルのハンドルID
	Size3DF scl;				// 大きさ
	Rot3DF rot;					// 回転
	Position3DF position;		// 位置
	Quaternion quaRot;			// 回転
	Quaternion quaRotLocal;		// ローカル回転

};

