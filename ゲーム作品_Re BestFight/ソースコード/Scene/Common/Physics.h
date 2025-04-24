#pragma once
#include<DxLib.h>
#include"../../Utility/VectorHeader.h"

class Physics
{
public:

	//重力のデフォルト方向
	static constexpr Dir3DF GRAVITY_DIR = {0.0f, -1.0f, 0.0f};
	//重力
	static constexpr float GRAVITY_POWER = 2.4f;
	//重力最大値
	static constexpr float GRAVITY_MAX = 180.0f;

	//摩擦力
	static constexpr float FRICTION_POWER = 7.6f;

	//コンストラクタ
	Physics(void);

	//デストラクタ
	~Physics(void);

	//重力処理
	static void Gravity(VECTOR& vec, float weight);

	//摩擦処理
	static void Friction(VECTOR& vec, float weight);

};