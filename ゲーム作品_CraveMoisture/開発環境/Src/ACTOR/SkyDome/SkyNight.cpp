#include "SkyNight.h"

SkyNight::SkyNight(void)
{
}

SkyNight::~SkyNight(void)
{
}

void SkyNight::InitSpecial(void)
{
}

void SkyNight::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "SkyDomeInfo.json", "NightSky" };
	actorTransform_ = std::make_shared<Transform>(info, ResourceManager::RESOURCE_ID::SKYDOME_NIGHT);
}

void SkyNight::UpdateSpecial(void)
{
}

void SkyNight::DrawSpecial(void)
{
}
