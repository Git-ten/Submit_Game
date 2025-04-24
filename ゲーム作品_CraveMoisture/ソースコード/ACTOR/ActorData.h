#pragma once
#include <memory>
#include "../../UTILITY/VectorHeader.h"

class ActorData
{
public:


	float maxHp_ = 100.0f;							// 最大体力
	float hp_ = 100.0f;								// 体力
	Position3DF* pos_;								// 座標
	std::shared_ptr<Line3DF>collitionGravity_;		// 重力用当たり判定

	std::shared_ptr<SphereF>collisionSphere_;		// 球当たり判定
	float collitionSphereSize_ = 0.0f;				// 球当たり判定のサイズ
	float collisionSphereOffset_ = 0.0f;			// 球当たり判定の中心のオフセット

	std::shared_ptr<SphereF>collisionAttackSphere_;	// 攻撃用当たり判定

	int speed_ = 0;									// 速度
	float attackPower_ = 0.0f;						// 攻撃力

};