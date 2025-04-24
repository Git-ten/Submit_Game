#pragma once
#include "../../Utility/VectorHeader.h"

class DataImage
{
public:

	int imgDataId;				// 画像のID
	Plane2D imgPlane;			// 画像の当たり判定用四角形
	Position2D posLeftUp;		// 左上座標
	Position2D posRightDown;	// 右下座標
	Position2D imgCenter;		// 中心座標
	double imgScl;				// スケール
	double imgAngle;			// 角度

};