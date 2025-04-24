#include <cassert>
#include <DxLIb.h>
#include "../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include "AnimationController.h"

#pragma region 定数宣言

//ブレンドの進行速度
//デルタタイムを割るので
//速くするためには、値を小さくする
const float BLEND_SPEED = 0.05f;

// ブレンドの最大値
const float BLEND_MAX = 1.0f;
// ブレンドの最小値
const float BLEND_MIN = 0.0f;
// 初期化子
const float INIT_ZERO = 0.0f;
// 符号判定用
const float DECISION_VALUE = 0.0f;
// モデルへのアタッチ番号
const int MODEL_ATTACH_NUM = 0;

#pragma endregion

AnimationController::AnimationController(const int& model) :
	modelId_(model),
	nowAnimation_(ANIMATION_INFO()),
	prevAnimation_(ANIMATION_INFO()),
	playAnimationFlag_(false),
	blendFlag_(false),
	blendRate_(BLEND_MIN),
	animSpeed_(INIT_ZERO)
{
}

AnimationController::~AnimationController()
{

}

void AnimationController::Update(bool isSlow)
{
	//アニメーションを再生していない
	if (!playAnimationFlag_)return;

	if (isSlow)
		animSpeed_ = 6.0f;
	else
		DefaultPlaySpeed();

	// ブレンド処理
	BlendAnimation();

	// アニメーション処理
	Animation();
}

void AnimationController::Add(int type, const DataAnimation& data)
{
	const auto& ite = animation_.find(type);

	//既に存在しているアニメーションは再登録しない
	if (ite != animation_.end())return;

	
	ANIMATION_INFO state = ANIMATION_INFO();

	int id = MV1LoadModel(data.path.c_str());
	state.nowType = type;
	state.modelId = id;
	state.speed = data.speed;
	state.step = data.start;
	state.isLoop = data.isLoop;
	state.start = data.start;
	state.end = data.end;
	animation_.emplace(type, state);
}

void AnimationController::Start(int type)
{
	const auto& ite = animation_.find(type);
	if (ite == animation_.end())return;
	

	nowAnimation_ = animation_.at(type);
	nowAnimation_.attachNo = MV1AttachAnim(modelId_, MODEL_ATTACH_NUM, nowAnimation_.modelId);
	nowAnimation_.totalTime = MV1GetAttachAnimTotalTime(modelId_, nowAnimation_.attachNo);
	animSpeed_ = nowAnimation_.speed;


	prevAnimation_ = nowAnimation_;
	playAnimationFlag_ = true;
}

void AnimationController::Play(int type)
{
	const auto& ite = animation_.find(type);
	if (ite == animation_.end())return;


	//現在再生している場合はアタッチ処理しない
	if (nowAnimation_.nowType == type)return;


	//現在ブレンド処理中の場合は強制的に終了
	if (blendFlag_)
	{
		blendFlag_ = false;
		blendRate_ = BLEND_MIN;
		MV1DetachAnim(modelId_, prevAnimation_.attachNo);
		MV1SetAttachAnimBlendRate(modelId_, nowAnimation_.attachNo, BLEND_MAX);
		prevAnimation_ = nowAnimation_;
	}

	// 再生処理
	SetUpPlayAnimation(type);
}

void AnimationController::RePlay(void)
{
	playAnimationFlag_ = true;
}

void AnimationController::Stop()
{
	//ブレンド中の場合
	//現在のアニメーションの前に再生していたアニメをデタッチする
	if (blendFlag_)
	{
		blendFlag_ = false;
		blendRate_ = BLEND_MIN;
		MV1DetachAnim(modelId_, prevAnimation_.attachNo);
		MV1DetachAnim(modelId_, nowAnimation_.attachNo);
		prevAnimation_ = nowAnimation_;
	}

	playAnimationFlag_ = false;
}

void AnimationController::ClearAnimMap()
{
	animation_.clear();
}

const bool AnimationController::IsPlayAnim()const
{
	return playAnimationFlag_;
}

const float AnimationController::GetStepAnimTime()const
{
	return nowAnimation_.step;
}

float& AnimationController::GetReferenceStepAnimTime(void)
{
	return nowAnimation_.step;
}

void AnimationController::SkipAnimTime(float skipStep)
{
	//現在のステップより前なら何もしない
	if (nowAnimation_.step > skipStep)return;

	nowAnimation_.step = skipStep;
}

const float AnimationController::GetEndStepAnimTime(void) const
{
	return nowAnimation_.end;
}

const int AnimationController::GetAttachAnimIndex(void) const
{
	return nowAnimation_.attachNo;
}

const bool AnimationController::IsBlendFlag()const
{
	return blendFlag_;
}

void AnimationController::EndBlend(void)
{
	// ブレンド率を強制的に100%にする
	blendRate_ = BLEND_MAX;
}

void AnimationController::SetPlaySpeed(float speed)
{
	animSpeed_ = speed;
}

void AnimationController::MultPlaySpeed(const float mul)
{
	animSpeed_ = nowAnimation_.speed * mul;
}

void AnimationController::DefaultPlaySpeed(void)
{
	animSpeed_ = nowAnimation_.speed;
}

void AnimationController::BlendAnimation(void)
{
	// ブレンド処理中でない
	if (!blendFlag_)return;

	const float deltatime = SceneManager::GetInstance().GetDeltaTime();

	blendRate_ += deltatime / BLEND_SPEED;
	MV1SetAttachAnimBlendRate(modelId_, prevAnimation_.attachNo, BLEND_MAX - blendRate_);
	MV1SetAttachAnimBlendRate(modelId_, nowAnimation_.attachNo, blendRate_);

	//ブレンド率が100パーセントになった場合
	if (blendRate_ >= BLEND_MAX)
	{
		//ブレンド終了処理
		MV1DetachAnim(modelId_, prevAnimation_.attachNo);
		//完全に現在のアニメーションをアタッチする
		MV1SetAttachAnimBlendRate(modelId_, nowAnimation_.attachNo, BLEND_MAX);


		// 初期化
		blendFlag_ = false;
		blendRate_ = BLEND_MIN;
		prevAnimation_ = nowAnimation_;
	}
}

void AnimationController::Animation(void)
{
	// ブレンド処理中は処理しない
	if (blendFlag_)return;

	const float deltatime = SceneManager::GetInstance().GetDeltaTime();
	//アニメーションのループ処理
	if (nowAnimation_.step > nowAnimation_.end)
	{
		if (nowAnimation_.isLoop)
		{
			//ループの最初から
			nowAnimation_.step = nowAnimation_.start;
		}
		else
		{
			//ループせずに終了
			playAnimationFlag_ = false;
			nowAnimation_.step = nowAnimation_.end;
		}
	}
	//次のアニメーション時間をセット
	MV1SetAttachAnimTime(modelId_, nowAnimation_.attachNo, nowAnimation_.step);
	nowAnimation_.step += animSpeed_ * deltatime;
}

void AnimationController::SetUpPlayAnimation(int type)
{
	nowAnimation_ = animation_.at(type);
	nowAnimation_.attachNo = MV1AttachAnim(modelId_, MODEL_ATTACH_NUM, nowAnimation_.modelId);
	nowAnimation_.totalTime = MV1GetAttachAnimTotalTime(modelId_, nowAnimation_.attachNo);
	animSpeed_ = nowAnimation_.speed;

	// 終了ステップは負の数を許容しない
	if (nowAnimation_.end < DECISION_VALUE)
	{
		nowAnimation_.end = nowAnimation_.totalTime;
	}

	playAnimationFlag_ = true;

	// 前回と同じアニメーションならブレンド処理しない
	if (prevAnimation_.modelId == nowAnimation_.modelId)return;


	//ブレンド用パラメータ
	blendFlag_ = true;
	blendRate_ = BLEND_MIN;
}