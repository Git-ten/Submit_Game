#include<DxLIb.h>
#include"../../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int& model) : modelId_(model)
{
	//現在再生しているアニメーション
	nowPlayAnim_ = ANIM_INFO();
	//切り替え前に再生していたアニメーション
	prevPlayAnim_ = ANIM_INFO();
	//アニメーションを再生しているか
	playAnimFlag_ = false;
	//ブレンド処理中か
	blendFlag_ = false;
	//ブレンド率
	blendRate_ = 0.0f;
}

AnimationController::~AnimationController()
{

}

void AnimationController::Init()
{

}

void AnimationController::Update()
{
	//アニメーションを再生していない
	if (!playAnimFlag_)
	{
		return;
	}

	float deltatime = SceneManager::GetInstance().GetDeltaTime();

	//ブレンド中である
	if (blendFlag_)
	{
		//ブレンド処理
		blendRate_ += deltatime / BLEND_SPEED;
		MV1SetAttachAnimBlendRate(modelId_, prevPlayAnim_.attachNo, 1.0f - blendRate_);
		MV1SetAttachAnimBlendRate(modelId_, nowPlayAnim_.attachNo, blendRate_);

		//ブレンド率が100パーセントになった場合
		if (blendRate_ >= 1.0f)
		{
			//ブレンド終了処理
			MV1DetachAnim(modelId_, prevPlayAnim_.attachNo);
			//完全に現在のアニメーションをアタッチする
			MV1SetAttachAnimBlendRate(modelId_, nowPlayAnim_.attachNo, 1.0f);

			blendFlag_ = false;
			blendRate_ = 0.0f;
			prevPlayAnim_ = nowPlayAnim_;
		}
	}
	//ブレンド中でない
	else
	{
		//アニメーションのループ処理
		if (nowPlayAnim_.step > nowPlayAnim_.end)
		{
			if (nowPlayAnim_.loopFlag)
			{
				//ループの最初から
				nowPlayAnim_.step = nowPlayAnim_.start;
			}
			else
			{
				//ループせずに終了
				playAnimFlag_ = false;
			}
		}
		//次のアニメーション時間をセット
		MV1SetAttachAnimTime(modelId_, nowPlayAnim_.attachNo, nowPlayAnim_.step);
		nowPlayAnim_.step += nowPlayAnim_.speed * deltatime;
	}
}

void AnimationController::Draw()
{

}

void AnimationController::Add(int type, const std::string& line, float speed, float start, float end, bool loop)
{
	auto ite = animation_.find(type);

	//既に存在しているアニメーションは再登録しない
	if (ite == animation_.end())
	{
		int id = MV1LoadModel(line.c_str());
		ANIM_INFO state = std::move(ANIM_INFO());

		state.modelId = id;
		state.speed = speed;
		state.step = start;
		state.loopFlag = loop;
		state.start = start;
		state.end = end;

		animation_.emplace(type, state);
	}
}

void AnimationController::Start(int type)
{
	auto ite = animation_.find(type);

	if (ite != animation_.end())
	{
		nowPlayAnim_ = animation_.at(type);
		nowPlayAnim_.attachNo = MV1AttachAnim(modelId_, 0, nowPlayAnim_.modelId);
		nowPlayAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, nowPlayAnim_.attachNo);

		prevPlayAnim_ = nowPlayAnim_;
		playAnimFlag_ = true;
	}
}

void AnimationController::Play(int type)
{
	auto ite = animation_.find(type);

	if (ite != animation_.end())
	{
		//現在再生している場合はアタッチ処理しない
		if (nowPlayAnim_.modelId == animation_.at(type).modelId)
		{
			return;
		}

		//現在ブレンド処理中の場合は強制的に終了
		if (blendFlag_)
		{
			blendFlag_ = false;
			blendRate_ = 0.0f;
			MV1DetachAnim(modelId_, prevPlayAnim_.attachNo);
			MV1SetAttachAnimBlendRate(modelId_, nowPlayAnim_.attachNo, 1.0f);
			prevPlayAnim_ = nowPlayAnim_;
		}

		nowPlayAnim_ = animation_.at(type);
		nowPlayAnim_.attachNo = MV1AttachAnim(modelId_, 0, nowPlayAnim_.modelId);
		nowPlayAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, nowPlayAnim_.attachNo);
		if (nowPlayAnim_.end < 0.0f)
		{
			nowPlayAnim_.end = nowPlayAnim_.totalTime;
		}

		playAnimFlag_ = true;

		//ブレンド処理をするかどうか
		if (prevPlayAnim_.modelId != nowPlayAnim_.modelId)
		{
			blendFlag_ = true;
			blendRate_ = 0.0f;
		}
	}
}

void AnimationController::Stop()
{
	//ブレンド中の場合
	//現在のアニメーションの前に再生していたアニメを停止する
	if (blendFlag_)
	{
		blendFlag_ = false;
		blendRate_ = 0.0f;
		MV1DetachAnim(modelId_, prevPlayAnim_.attachNo);
		MV1DetachAnim(modelId_, nowPlayAnim_.attachNo);
		prevPlayAnim_ = nowPlayAnim_;
	}

	playAnimFlag_ = false;
}

void AnimationController::ClearAnimMap()
{
	animation_.clear();
}

bool AnimationController::IsPlayAnim()const
{
	return playAnimFlag_;
}

float AnimationController::GetStepAnimTime()const
{
	return nowPlayAnim_.step;
}

bool AnimationController::IsBlendFlag()const
{
	return blendFlag_;
}