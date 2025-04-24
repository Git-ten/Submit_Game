#pragma once
#include "SkyDomeBase.h"
class SkySun : public SkyDomeBase
{
public:

	SkySun(void);
	~SkySun(void)override;

	void InitSpecial(void)override;

protected:

	void InitActorTransform(void)override;

	void UpdateSpecial(void)override;
	void DrawSpecial(void)override;
};