#include "SpliteAnime.h"

#pragma region íËêîêÈåæ

const int DEFAULT_FRAME = 30;
const float DEFAULT_SPEED = 0.5f;

#pragma endregion

SpliteAnime::SpliteAnime(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id):
	nowNumber_(0),
	frame_(DEFAULT_FRAME),
	speed_(DEFAULT_SPEED),
	animationCounter_(0.0f),
	isPlayAnime_(false),
	isLoopAnime_(false)
{
	imageAnime_ = std::make_unique<ImageDiv>(info, id);
}

SpliteAnime::SpliteAnime(const LoadData::LoadDataInfo& info, const EffectResourceManager::EFFECT_RESOURCE_ID& id) :
	nowNumber_(0),
	frame_(DEFAULT_FRAME),
	speed_(DEFAULT_SPEED),
	animationCounter_(0.0f),
	isPlayAnime_(false),
	isLoopAnime_(false)
{
	imageAnime_ = std::make_unique<ImageDiv>(info, id);
}

SpliteAnime::~SpliteAnime(void)
{
}

void SpliteAnime::Update(void)
{
	if (!isPlayAnime_)return;

	if (static_cast<int>(animationCounter_) % frame_ == 0)
	{
		nowNumber_++;
		if (nowNumber_ >= imageAnime_->GetImageDataSize())
		{
			if(!isLoopAnime_)
				isPlayAnime_ = false;

			nowNumber_ = 0;
			animationCounter_ = 0.0f;
		}
	}

	animationCounter_ += speed_ * static_cast<float>(frame_);
}

void SpliteAnime::Draw(void)
{
	if (!isPlayAnime_)return;

	imageAnime_->DrawImageDiv(nowNumber_);
}

void SpliteAnime::PlaySpliteAnime(bool isLoop)
{
	isLoopAnime_ = isLoop;
	isPlayAnime_ = true;
}

void SpliteAnime::SetUpAnimePos(const Position2D& pos)
{
	imageAnime_->SetPos(pos);
}

void SpliteAnime::EndAnime(void)
{
	isLoopAnime_ = false;
	isPlayAnime_ = false;
}

const Position2D& SpliteAnime::GetPos(void)
{
	return imageAnime_->GetPos();
}

Position2D& SpliteAnime::GetPosReference(void)
{
	return imageAnime_->GetPosReference();
}

Plane2D& SpliteAnime::GetPlaneReference(void)
{
	return imageAnime_->GetPlaneReference();
}

bool SpliteAnime::IsPlayAnime(void)
{
	return isPlayAnime_;
}
