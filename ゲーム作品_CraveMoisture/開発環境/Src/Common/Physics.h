#pragma once
#include<DxLib.h>
#include "../../Utility/Utility.h"
#include"../../Utility/VectorHeader.h"

class Physics
{
public:

	//重力のデフォルト方向
	static constexpr Dir3DF GRAVITY_DIR = {0.0f, -1.0f, 0.0f};
	//重力の最小値
	static constexpr float GRAVITY_MIN = 5.0f;
	//摩擦力の最小値
	static constexpr float SWAY_MIN = 40.0f;


	//重力処理
	static void Gravity(VECTOR& vec, float weight = 0.0f)
	{
		//重力最大値
		const float GRAVITY_MAX = 180.0f;
		//重力
		const float GRAVITY_POWER = 2.4f;

		VECTOR power = VScale(GRAVITY_DIR, GRAVITY_POWER + weight);
		vec = VAdd(vec, power);

		//下限
		if (vec.y < -GRAVITY_MAX)
		{
			vec.y = -GRAVITY_MAX;
		}
	}

	//摩擦処理
	static void Inertia(VECTOR& vec, float weight = 0.0f)
	{
		//摩擦力
		const float FRICTION_POWER = 0.8f;

		if (Utility::EqualsVZero(vec))return;

		//方向ベクトルを取得
		Dir3DF dir = VNorm(vec);
		//逆方向
		dir = VScale(dir, -1.0f);

		VECTOR power = VScale(dir, FRICTION_POWER + weight);

		vec = VAdd(vec, power);

		//同じ方向になったら0にする
		if (VDot(dir, vec) > 0.0f)
		{
			vec = Utility::VECTOR_ZERO;
		}
	}

};