#include <DxLib.h>
#include "SoundResource.h"

#pragma region ’è”éŒ¾

// —áŠOq
const int EXCEPTION_VALUE = -1;

#pragma endregion

SoundResource::SoundResource(void):
	path_(""),
	handleId_(EXCEPTION_VALUE)
{
}

SoundResource::SoundResource(const std::string& path):
	path_(path),
	handleId_(EXCEPTION_VALUE)
{
}

SoundResource::~SoundResource(void)
{
}

void SoundResource::Release(void)
{
	DeleteSoundMem(handleId_);
}

void SoundResource::Release(int handleId)
{
	DeleteSoundMem(handleId);
}

void SoundResource::Load(void)
{
	handleId_ = LoadSoundMem(path_.c_str());
}