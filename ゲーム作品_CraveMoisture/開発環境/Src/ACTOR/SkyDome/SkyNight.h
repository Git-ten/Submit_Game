#pragma once
#include "SkyDomeBase.h"
class SkyNight : public SkyDomeBase
{
public:

	SkyNight(void);
	~SkyNight(void)override;

	void InitSpecial(void)override;

protected:

	void InitActorTransform(void)override;

	void UpdateSpecial(void)override;
	void DrawSpecial(void)override;
};