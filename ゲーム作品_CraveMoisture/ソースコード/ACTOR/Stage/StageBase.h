#pragma once
#include "../ActorBase.h"
class StageBase :public ActorBase
{
public:

	StageBase(void);
	virtual ~StageBase(void);

protected:

	virtual void InitActorTransform(void)override;

	virtual void InitSpecial(void)override;
	virtual void UpdateSpecial(void)override;

	virtual void CollisionedGravityActor(Position3DF pos)override;

	virtual void DrawSpecial(void)override;

};