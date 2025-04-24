#include "SkySun.h"

SkySun::SkySun(void)
{
}

SkySun::~SkySun(void)
{
}

void SkySun::InitSpecial(void)
{
	LoadData::LoadDataInfo info = { "SkyDomeInfo.json", "SunSky" };
	actorTransform_ = std::make_shared<Transform>(info, ResourceManager::RESOURCE_ID::SKYDOME_SUN);
}

void SkySun::InitActorTransform(void)
{
}

void SkySun::UpdateSpecial(void)
{
}

void SkySun::DrawSpecial(void)
{
}
