#include <DxLib.h>
#include <cmath>
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Application/Application.h"
#include "../../UTILITY/Utility.h"
#include "Image.h"

#pragma region 定数宣言

// 透明度の最大値
const float ALPHA_MAX = 1.0f;

// 透明度の最小値
const float ALPHA_MIN = 0.0f;

// 画像上の左上座標
const Position2D SRC_POS_LEFT = { 0,0 };

// ゲージの最小値(パーセント)
const double GAGE_MIN = 0.0;

#pragma endregion

Image::Image(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id,
	const IMAGE_TYPE& type, const int addId) :
	alpha_(ALPHA_MAX),
	resId_(id),
	imageType_(type),
	imageGageRate_(nullptr),
	imageGageMaxRate_(0.0f),
	imageGageColor_({0, 0, 0}),
	addResource_(addId),
	prevImageGageRate_(-1.0f)
{
	imgInfo_= LoadDataImage::LoadedDataImage(Application::GetInstance().GetDataPaths(), id, info);
}

Image::~Image(void)
{
}

void Image::SetUpImageGageRate(float& gage, float maxGage, Color3D color, float addAngle)
{
	imageGageRate_ = &gage;
	imageGageMaxRate_ = maxGage;
	imageGageColor_ = color;

	addImageGageAngle_ = addAngle;
}

void Image::SetUpImageEventGageRate(float gage)
{
	prevImageGageRate_ = gage;
}

void Image::AnimationImage(const std::function<void(void)>& animation)
{
	animation();
}

void Image::DrawImage(int modelId)
{
	if (modelId < 0)
		modelId = imgInfo_.imgDataId;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * alpha_));
	DrawRotaGraph(imgInfo_.imgCenter.x, imgInfo_.imgCenter.y,
		imgInfo_.imgScl, imgInfo_.imgAngle, modelId, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Image::DrawRectImage(const Position2D& rightPos, const Size2D& size)
{
	DrawRectExtendGraph(imgInfo_.posLeftUp.x, imgInfo_.posLeftUp.y,
		rightPos.x, rightPos.y,
		SRC_POS_LEFT.x, SRC_POS_LEFT.y,
		size.x, size.y,
		imgInfo_.imgDataId, true);
}

void Image::DrawGageImage(const Color3D& color)
{
	Color3D nowColor = color;

	double gageRate = 1.0f;
	if (imageGageRate_ != nullptr &&
		imageGageMaxRate_ > 0.0f)gageRate = static_cast<double>((*imageGageRate_ / imageGageMaxRate_));

	if (gageRate < 0.0)return;

	if (gageRate >= 1.0f)
		DrawImage(addResource_);
	else
		DrawImage();

	if (prevImageGageRate_ >= 0.0f)
	{
		const float GAGE_SPEED = 2.5f;
		gageRate = 1.0 - static_cast<double>(prevImageGageRate_ / imageGageMaxRate_) * 2.0;
		if (*imageGageRate_ < prevImageGageRate_)
		{
			nowColor = { 255, 0, 0 };
			prevImageGageRate_ -= GAGE_SPEED;
		}
		else if (*imageGageRate_ > prevImageGageRate_)
		{
			nowColor = { 0, 255, 0 };
			prevImageGageRate_ += GAGE_SPEED;
		}

		float sub = prevImageGageRate_ - *imageGageRate_;

		if (sub < 0.0f)sub *= -1.0f;
		if (sub < GAGE_SPEED || sub > 50.0f)prevImageGageRate_ = *imageGageRate_;
	}
	else
		gageRate = 1.0 - gageRate * 2.0;

	const int GAGE_ALPHA = 128;

	//vxcosθ − vysinθ
	//vxsinθ + vycosθ
	auto mulVec = [&](const Vector2& vec, double rate = 1.0)
		{
			double angle = (imgInfo_.imgAngle + addImageGageAngle_) * -1.0;

			Vector2 cVec = { vec.x - imgInfo_.imgCenter.x, vec.y - imgInfo_.imgCenter.y };
			Vector2F aVec = { 0.0 + static_cast<double>(cVec.x) * rate, 0.0 + static_cast<double>(cVec.y) };
			double X = aVec.x * cos(angle) - aVec.y * sin(angle);
			double Y = aVec.x * sin(angle) + aVec.y * cos(angle);
			aVec.x = imgInfo_.imgCenter.x - X;
			aVec.y = imgInfo_.imgCenter.y - Y;
			return aVec;
		};

	Vector2F vec1 = mulVec({ imgInfo_.posLeftUp.x, imgInfo_.posLeftUp.y - 3 });
	Vector2F vec2 = mulVec({ imgInfo_.posRightDown.x, imgInfo_.posRightDown.y + 3 }, gageRate);

	SetDrawArea(static_cast<int>(vec1.x), static_cast<int>(vec1.y),
		static_cast<int>(vec2.x), static_cast<int>(vec2.y));
	SetDrawBright(nowColor.x, nowColor.y, nowColor.z);
	DrawImage();
	SetDrawBright(255, 255, 255);
	SetDrawAreaFull();
}

void Image::DrawCircleImage(const float rate)
{
	int modelId = imgInfo_.imgDataId;
	if (rate >= 1.0f)
		modelId = addResource_;

	DrawCircleGauge(imgInfo_.imgCenter.x, imgInfo_.imgCenter.y, 
		static_cast<double>(rate) * 100.0, modelId,
		GAGE_MIN, imgInfo_.imgScl);
}

void Image::DrawTypeImage(void)
{
	switch (imageType_)
	{
	case IMAGE_TYPE::NORMAL:
		DrawImage();
		break;
	case IMAGE_TYPE::GAGE:
		DrawGageImage(imageGageColor_);
		break;
	case IMAGE_TYPE::CIRCLE_GAGE:
		DrawCircleImage(*imageGageRate_ / imageGageMaxRate_);
		break;
	default:
		break;
	}
}

void Image::SetAlpha(const float alpha)
{
	alpha_ = alpha;
}

void Image::SetUpCollision(void)
{
	imgInfo_.imgPlane = { imgInfo_.posLeftUp, imgInfo_.posRightDown };
}

void Image::SetPos(const Position2D& pos)
{
	imgInfo_.imgCenter = pos;
}

const ResourceManager::RESOURCE_ID& Image::GetNowId(void)const
{
	return resId_;
}

void Image::ChangeImage(const ResourceManager::RESOURCE_ID& id)
{
	resId_ = id;
	imgInfo_.imgDataId = ResourceManager::GetInstance().Load(id).handleId_;
}

const int Image::GetId(void) const
{
	return imgInfo_.imgDataId;
}

const Position2D& Image::GetLeftUpPos(void) const
{
	return imgInfo_.posLeftUp;
}

const Position2D& Image::GetRightDownPos(void) const
{
	return imgInfo_.posRightDown;
}

const Position2D& Image::GetCenterPos(void) const
{
	return imgInfo_.imgCenter;
}

const Size2D Image::GetSize(void)
{
	Size2D size = imgInfo_.posRightDown;
	size = size - imgInfo_.posLeftUp;
	return size;
}

const double Image::GetScl(void) const
{
	return imgInfo_.imgScl;
}

const double Image::GetAngle(void) const
{
	return imgInfo_.imgAngle;
}

const Plane2D& Image::GetCollPlane(void) const
{
	return imgInfo_.imgPlane;
}
