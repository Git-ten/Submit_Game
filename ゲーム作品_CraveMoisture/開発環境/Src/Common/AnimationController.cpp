#include <cassert>
#include <DxLIb.h>
#include "../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include "AnimationController.h"

#pragma region �萔�錾

//�u�����h�̐i�s���x
//�f���^�^�C��������̂�
//�������邽�߂ɂ́A�l������������
const float BLEND_SPEED = 0.05f;

// �u�����h�̍ő�l
const float BLEND_MAX = 1.0f;
// �u�����h�̍ŏ��l
const float BLEND_MIN = 0.0f;
// �������q
const float INIT_ZERO = 0.0f;
// ��������p
const float DECISION_VALUE = 0.0f;
// ���f���ւ̃A�^�b�`�ԍ�
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
	//�A�j���[�V�������Đ����Ă��Ȃ�
	if (!playAnimationFlag_)return;

	if (isSlow)
		animSpeed_ = 6.0f;
	else
		DefaultPlaySpeed();

	// �u�����h����
	BlendAnimation();

	// �A�j���[�V��������
	Animation();
}

void AnimationController::Add(int type, const DataAnimation& data)
{
	const auto& ite = animation_.find(type);

	//���ɑ��݂��Ă���A�j���[�V�����͍ēo�^���Ȃ�
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


	//���ݍĐ����Ă���ꍇ�̓A�^�b�`�������Ȃ�
	if (nowAnimation_.nowType == type)return;


	//���݃u�����h�������̏ꍇ�͋����I�ɏI��
	if (blendFlag_)
	{
		blendFlag_ = false;
		blendRate_ = BLEND_MIN;
		MV1DetachAnim(modelId_, prevAnimation_.attachNo);
		MV1SetAttachAnimBlendRate(modelId_, nowAnimation_.attachNo, BLEND_MAX);
		prevAnimation_ = nowAnimation_;
	}

	// �Đ�����
	SetUpPlayAnimation(type);
}

void AnimationController::RePlay(void)
{
	playAnimationFlag_ = true;
}

void AnimationController::Stop()
{
	//�u�����h���̏ꍇ
	//���݂̃A�j���[�V�����̑O�ɍĐ����Ă����A�j�����f�^�b�`����
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
	//���݂̃X�e�b�v���O�Ȃ牽�����Ȃ�
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
	// �u�����h���������I��100%�ɂ���
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
	// �u�����h�������łȂ�
	if (!blendFlag_)return;

	const float deltatime = SceneManager::GetInstance().GetDeltaTime();

	blendRate_ += deltatime / BLEND_SPEED;
	MV1SetAttachAnimBlendRate(modelId_, prevAnimation_.attachNo, BLEND_MAX - blendRate_);
	MV1SetAttachAnimBlendRate(modelId_, nowAnimation_.attachNo, blendRate_);

	//�u�����h����100�p�[�Z���g�ɂȂ����ꍇ
	if (blendRate_ >= BLEND_MAX)
	{
		//�u�����h�I������
		MV1DetachAnim(modelId_, prevAnimation_.attachNo);
		//���S�Ɍ��݂̃A�j���[�V�������A�^�b�`����
		MV1SetAttachAnimBlendRate(modelId_, nowAnimation_.attachNo, BLEND_MAX);


		// ������
		blendFlag_ = false;
		blendRate_ = BLEND_MIN;
		prevAnimation_ = nowAnimation_;
	}
}

void AnimationController::Animation(void)
{
	// �u�����h�������͏������Ȃ�
	if (blendFlag_)return;

	const float deltatime = SceneManager::GetInstance().GetDeltaTime();
	//�A�j���[�V�����̃��[�v����
	if (nowAnimation_.step > nowAnimation_.end)
	{
		if (nowAnimation_.isLoop)
		{
			//���[�v�̍ŏ�����
			nowAnimation_.step = nowAnimation_.start;
		}
		else
		{
			//���[�v�����ɏI��
			playAnimationFlag_ = false;
			nowAnimation_.step = nowAnimation_.end;
		}
	}
	//���̃A�j���[�V�������Ԃ��Z�b�g
	MV1SetAttachAnimTime(modelId_, nowAnimation_.attachNo, nowAnimation_.step);
	nowAnimation_.step += animSpeed_ * deltatime;
}

void AnimationController::SetUpPlayAnimation(int type)
{
	nowAnimation_ = animation_.at(type);
	nowAnimation_.attachNo = MV1AttachAnim(modelId_, MODEL_ATTACH_NUM, nowAnimation_.modelId);
	nowAnimation_.totalTime = MV1GetAttachAnimTotalTime(modelId_, nowAnimation_.attachNo);
	animSpeed_ = nowAnimation_.speed;

	// �I���X�e�b�v�͕��̐������e���Ȃ�
	if (nowAnimation_.end < DECISION_VALUE)
	{
		nowAnimation_.end = nowAnimation_.totalTime;
	}

	playAnimationFlag_ = true;

	// �O��Ɠ����A�j���[�V�����Ȃ�u�����h�������Ȃ�
	if (prevAnimation_.modelId == nowAnimation_.modelId)return;


	//�u�����h�p�p�����[�^
	blendFlag_ = true;
	blendRate_ = BLEND_MIN;
}