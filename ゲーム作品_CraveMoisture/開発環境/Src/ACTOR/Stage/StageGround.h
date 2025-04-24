#pragma once
#include "StageBase.h"
class StageGround : public StageBase
{
public:

	// コンストラクタ
	StageGround(void);
	// デストラクタ
	~StageGround(void)override;

	void InitSpecial(void)override;

	void Draw(void)override;

	// コールバック関数
	bool BattleStepCondition(void)override;

private:

	void InitActorTransform(void)override;

	void UpdateSpecial(void)override;

	void CollisionedGravityActor(Position3DF pos)override;

};