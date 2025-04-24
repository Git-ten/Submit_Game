#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "EffectResource.h"

#pragma region 定数宣言

// 例外子
const int EXCEPTION_VALUE = -1;

#pragma endregion

EffectResource::EffectResource(void) :
	resType_(EFFECT_TYPE::NONE),
	path_(""),
	handleId_(EXCEPTION_VALUE)
{
}

EffectResource::EffectResource(const EFFECT_TYPE& type, const std::string& path) :
	resType_(type),
	path_(path),
	handleId_(EXCEPTION_VALUE)
{
}

EffectResource::EffectResource(const EFFECT_TYPE& type, const std::string& path,
	int numX, int numY, int sizeX, int sizeY):
	resType_(type),
	path_(path),
	handleId_(EXCEPTION_VALUE),
	numX_(numX),
	numY_(numY),
	sizeX_(sizeX),
	sizeY_(sizeY)
{
}

EffectResource::~EffectResource(void)
{
}

void EffectResource::Release(void)
{
	switch (resType_)
	{
	case EFFECT_TYPE::EFFEKSEER_3D:
	case EFFECT_TYPE::EFFEKSEER_2D:
		DeleteEffekseerEffect(handleId_);
		break;
	case EFFECT_TYPE::SPLITE:
		// 複数画像
		handleIds_.resize(numX_ * numY_);
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;
	default:
		break;
	}
}

void EffectResource::Release(int handleId)
{
	switch (resType_)
	{
	case EFFECT_TYPE::EFFEKSEER_3D:
	case EFFECT_TYPE::EFFEKSEER_2D:
		DeleteEffekseerEffect(handleId_);
		break;
	case EFFECT_TYPE::SPLITE:
	{
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		handleIds_.clear();
	}
		break;
	default:
		break;
	}
}

void EffectResource::Load(void)
{
	switch (resType_)
	{
	case EFFECT_TYPE::EFFEKSEER_3D:
	case EFFECT_TYPE::EFFEKSEER_2D:
		// エフェクト
		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;
	case EFFECT_TYPE::SPLITE:
		// 複数画像
		handleIds_.resize(numX_ * numY_);
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;
	default:
		break;
	}
}