#include"../../Utility/MyUtility.h"
#include "Physics.h"

//コンストラクタ
Physics::Physics()
{

}

//デストラクタ
Physics::~Physics()
{

}

void Physics::Gravity(VECTOR& vec, float weight)
{
	//0ぴったりでは計算できないので、補正する
	if (MyUtility::EqualsVZero(vec))vec.y = 1.0f;
	VECTOR power = VScale(GRAVITY_DIR, GRAVITY_POWER + weight);
	vec = VAdd(vec, power);

	//下限
	if (vec.y < -GRAVITY_MAX)
	{
		vec.y = -GRAVITY_MAX;
	}
}

void Physics::Friction(VECTOR& vec, float weight)
{
	if (MyUtility::EqualsVZero(vec))return;

	//方向ベクトルを取得
	Dir3DF dir = VNorm(vec);
	//逆方向
	dir = VScale(dir, -1.0f);

	VECTOR power = VScale(dir, FRICTION_POWER + weight);

	vec = VAdd(vec, power);

	//同じ方向になったら0にする
	if (VDot(dir, vec) > 0.0f)
	{
		vec = MyUtility::VECTOR_ZERO;
	}
}