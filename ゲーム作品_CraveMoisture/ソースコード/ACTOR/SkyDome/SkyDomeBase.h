#pragma once
#include "../ActorBase.h"
class SkyDomeBase : public ActorBase
{
public:

	SkyDomeBase(void);
	virtual ~SkyDomeBase(void)override;

	virtual void InitSpecial(void)override;

protected:

	virtual void InitActorTransform(void)override;

	virtual void UpdateSpecial(void)override;
	virtual void DrawSpecial(void)override;

};