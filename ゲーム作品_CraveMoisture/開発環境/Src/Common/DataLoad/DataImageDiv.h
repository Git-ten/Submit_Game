#pragma once
#include <vector>
#include "../../Utility/VectorHeader.h"

class DataImageDiv
{
public:

	std::vector<int>imgDataId;	// 複数画像のID
	Plane2D imgPlane;			// 画像の当たり判定用四角形
	Position2D posLeftUp;		// 左上座標
	Position2D posRightDown;	// 右下座標
	Position2D imgCenter;		// 中心座標
	double imgScl;				// スケール
	double imgAngle;			// 角度

};