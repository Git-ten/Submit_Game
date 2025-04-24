#include <DxLib.h>
#include<EffekseerForDxLib.h>
#include "Effect.h"

Effect::Effect(void)
{
	type_ = TYPE::NONE;
	path_ = "";
	resHandleId_ = -1;
	playHandleId_ = -1;
	pos_ = Position3DF();
}

Effect::Effect(const TYPE& type, std::string path)
{
	type_ = type;
	path_ = path;
	resHandleId_ = -1;
	playHandleId_ = -1;
	pos_ = Position3DF();
}

Effect::~Effect()
{

}

void Effect::Release()
{
	switch (type_)
	{
	case TYPE::EFFKSEER3D:
	case TYPE::EFFKSEER2D:
		DeleteEffekseerEffect(resHandleId_);
		break;
	}
}

void Effect::Load()
{
	switch (type_)
	{
	case TYPE::EFFKSEER3D:
	case TYPE::EFFKSEER2D:
		resHandleId_ = LoadEffekseerEffect(path_.c_str());
		playHandleId_ = -1;
		break;
	default:
		break;
	}
}

void Effect::Play()
{
	if (IsCheckPlay())
	{
		Stop();
	}

	switch (type_)
	{
	case Effect::TYPE::EFFKSEER3D:
		playHandleId_ = PlayEffekseer3DEffect(resHandleId_);
		break;
	case Effect::TYPE::EFFKSEER2D:
		playHandleId_ = PlayEffekseer2DEffect(resHandleId_);
		break;
	default:
		break;
	}
}

void Effect::Stop()
{
	if (IsCheckPlay())
	{
		switch (type_)
		{
		case Effect::TYPE::EFFKSEER3D:
			StopEffekseer3DEffect(playHandleId_);
			break;
		case Effect::TYPE::EFFKSEER2D:
			StopEffekseer2DEffect(playHandleId_);
			break;
		default:
			break;
		}
	}
}

bool Effect::IsCheckPlay()const
{
	switch (type_)
	{
	case Effect::TYPE::EFFKSEER3D:
		return IsEffekseer3DEffectPlaying(playHandleId_) == 0 ? true : false;
		break;
	case Effect::TYPE::EFFKSEER2D:
		return IsEffekseer2DEffectPlaying(playHandleId_) == 0 ? true : false;
		break;
	default:
		return false;
		break;
	}
}

void Effect::SetPos(Position3DF pos)
{
	if (IsCheckPlay())
	{
		pos_ = pos;
		switch (type_)
		{
		case Effect::TYPE::EFFKSEER3D:
			SetPosPlayingEffekseer3DEffect(playHandleId_, pos.x, pos.y, pos.z);
			break;
		case Effect::TYPE::EFFKSEER2D:
			SetPosPlayingEffekseer2DEffect(playHandleId_, pos.x, pos.y, pos.z);
			break;
		default:
			break;
		}
	}
}

void Effect::SetRot(Rot3DF rot)
{
	if (IsCheckPlay())
	{
		switch (type_)
		{
		case Effect::TYPE::EFFKSEER3D:
			SetRotationPlayingEffekseer3DEffect(playHandleId_, rot.x, rot.y, rot.z);
			break;
		case Effect::TYPE::EFFKSEER2D:
			SetRotationPlayingEffekseer2DEffect(playHandleId_, rot.x, rot.y, rot.z);
			break;
		default:
			break;
		}
	}
}

void Effect::SetScl(Size3DF scl)
{
	if (IsCheckPlay())
	{
		switch (type_)
		{
		case Effect::TYPE::EFFKSEER3D:
			SetScalePlayingEffekseer3DEffect(playHandleId_, scl.x, scl.y, scl.z);
			break;
		case Effect::TYPE::EFFKSEER2D:
			SetScalePlayingEffekseer2DEffect(playHandleId_, scl.x, scl.y, scl.z);
			break;
		default:
			break;
		}
	}
}

void Effect::SetSpeed(float speed)
{
	if (IsCheckPlay())
	{
		switch (type_)
		{
		case Effect::TYPE::EFFKSEER3D:
			SetSpeedPlayingEffekseer3DEffect(playHandleId_, speed);
			break;
		case Effect::TYPE::EFFKSEER2D:
			SetSpeedPlayingEffekseer2DEffect(playHandleId_, speed);
			break;
		default:
			break;
		}
	}
}

void Effect::SetRGB(int r, int g, int b, int a)
{
	if (IsCheckPlay())
	{
		switch (type_)
		{
		case Effect::TYPE::EFFKSEER3D:
			SetColorPlayingEffekseer3DEffect(playHandleId_, r, g, b, a);
			break;
		case Effect::TYPE::EFFKSEER2D:
			SetColorPlayingEffekseer2DEffect(playHandleId_, r, g, b, a);
			break;
		default:
			break;
		}
	}
}

Position3DF Effect::GetPos(void) const
{
	return pos_;
}