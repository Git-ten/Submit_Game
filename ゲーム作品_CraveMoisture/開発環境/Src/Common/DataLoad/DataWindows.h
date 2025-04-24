#pragma once
#include "../../Utility/VectorHeader.h"

class DataWindows
{
public:

	Size2D screenSize;		// スクリーンサイズ
	Color3D screenColor;	// スクリーンの中心座標
	float frameFps;			// FPS
	bool isWindowMode;		// フルスクリーンであるか
	bool isMouseShow;		// マウスカーソルが表示されているか

};

