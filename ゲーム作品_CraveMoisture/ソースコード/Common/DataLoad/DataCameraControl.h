#pragma once
#include <string>
#include "../../Utility/VectorHeader.h"

class DataCameraControl
{
public:

	float distance;			// カメラの距離
	float speed;			// カメラの駆動速度
	Position3DF targetPos;	// 視点座標
	Rot3DF angleRot;		// カメラの角度
	std::string eventName;	// 行うイベントの名前
	float eventTime;		// カメラ演出のタイム

};