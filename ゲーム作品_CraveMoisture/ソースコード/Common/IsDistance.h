#pragma once
#include <memory>
#include "../../Utility/VectorHeader.h"

class IsDistance
{
public:

	// 近距離判定
	static constexpr float CONDITION_NEAR_DISTANCE = 1500.0f;
	// 超近距離判定
	static constexpr float CONDITION_MOST_NEAR_DISTANCE = 500.0f;
	// 中距離判定
	static constexpr float CONDITION_MEDIUM_DISTANCE = 3000.0;
	// 遠距離判定
	static constexpr float CONDITION_FAR_DISTANCE = 5000.0;


	// コンストラクタ
	IsDistance(Position3DF& pos, Position3DF& targetPos):
		targetPos_(targetPos),
		pos_(pos)
	{
	}
	// デストラクタ
	~IsDistance(void)
	{
	}

	//近距離であるか
	const bool IsNearDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_NEAR_DISTANCE;
	}
	//触れる寸前なほど近い
	const bool IsMostNearDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_MOST_NEAR_DISTANCE;
	}
	//中距離であるか
	const bool IsMediumDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_MEDIUM_DISTANCE;
	}
	//遠距離であるか
	const bool IsFarDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_FAR_DISTANCE;
	}

	// 相手アクターと使用者アクターとの距離
	const float DistanceActors(void)
	{
		return VSize(VSub(targetPos_, pos_));
	}

	// 相手アクターから使用者アクターまで方向ベクトル
	const Dir3DF DirShareActorToActor(void)
	{
		return VNorm(VSub(targetPos_, pos_));
	}

private:

	// 相手アクターの情報
	const Position3DF& targetPos_;

	// 使用者アクターの座標
	const Position3DF& pos_;

	// 

};