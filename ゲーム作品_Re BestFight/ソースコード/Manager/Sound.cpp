#include<DxLib.h>
#include "Sound.h"

#define DEBUG_SOUND (1)

Sound::Sound(void)
{
	type_ = TYPE::NONE;
	path_ = "";
}

Sound::Sound(const TYPE& type, std::string path)
{
	type_ = type;
	path_ = path;
}

void Sound::Load(void)
{
#ifdef DEBUG_SOUND
	switch (type_) 
	{
	case Sound::TYPE::MUSIC:
	case Sound::TYPE::WAVE:
	{
		handleId_ = LoadSoundMem(path_.c_str());
	}
		break;
	}
#endif
}

void Sound::Play(int type)
{
//DX_PLAYTYPE_NORMAL　:　ノーマル再生
//DX_PLAYTYPE_BACK　　 : バックグラウンド再生
//DX_PLAYTYPE_LOOP　　 : ループ再生
#ifdef DEBUG_SOUND
	PlaySoundMem(handleId_, type);
#endif
}

void Sound::Stop(void)
{
#ifdef DEBUG_SOUND
	StopSoundMem(handleId_);
#endif
}

bool Sound::IsCheckPlay(void) const
{
	return CheckSoundMem(handleId_) <= 0 ? false : true;
}

void Sound::Release(void)
{
#ifdef DEBUG_SOUND
	switch (type_)
	{
	case Sound::TYPE::MUSIC:
	case Sound::TYPE::WAVE:
		StopSoundMem(handleId_);
		DeleteSoundMem(handleId_);
		break;
	}
#endif
}