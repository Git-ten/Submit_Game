#include<DxLIb.h>
#include"../../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int& model) : modelId_(model)
{
	//���ݍĐ����Ă���A�j���[�V����
	nowPlayAnim_ = ANIM_INFO();
	//�؂�ւ��O�ɍĐ����Ă����A�j���[�V����
	prevPlayAnim_ = ANIM_INFO();
	//�A�j���[�V�������Đ����Ă��邩
	playAnimFlag_ = false;
	//�u�����h��������
	blendFlag_ = false;
	//�u�����h��
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
	//�A�j���[�V�������Đ����Ă��Ȃ�
	if (!playAnimFlag_)
	{
		return;
	}

	float deltatime = SceneManager::GetInstance().GetDeltaTime();

	//�u�����h���ł���
	if (blendFlag_)
	{
		//�u�����h����
		blendRate_ += deltatime / BLEND_SPEED;
		MV1SetAttachAnimBlendRate(modelId_, prevPlayAnim_.attachNo, 1.0f - blendRate_);
		MV1SetAttachAnimBlendRate(modelId_, nowPlayAnim_.attachNo, blendRate_);

		//�u�����h����100�p�[�Z���g�ɂȂ����ꍇ
		if (blendRate_ >= 1.0f)
		{
			//�u�����h�I������
			MV1DetachAnim(modelId_, prevPlayAnim_.attachNo);
			//���S�Ɍ��݂̃A�j���[�V�������A�^�b�`����
			MV1SetAttachAnimBlendRate(modelId_, nowPlayAnim_.attachNo, 1.0f);

			blendFlag_ = false;
			blendRate_ = 0.0f;
			prevPlayAnim_ = nowPlayAnim_;
		}
	}
	//�u�����h���łȂ�
	else
	{
		//�A�j���[�V�����̃��[�v����
		if (nowPlayAnim_.step > nowPlayAnim_.end)
		{
			if (nowPlayAnim_.loopFlag)
			{
				//���[�v�̍ŏ�����
				nowPlayAnim_.step = nowPlayAnim_.start;
			}
			else
			{
				//���[�v�����ɏI��
				playAnimFlag_ = false;
			}
		}
		//���̃A�j���[�V�������Ԃ��Z�b�g
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

	//���ɑ��݂��Ă���A�j���[�V�����͍ēo�^���Ȃ�
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
		//���ݍĐ����Ă���ꍇ�̓A�^�b�`�������Ȃ�
		if (nowPlayAnim_.modelId == animation_.at(type).modelId)
		{
			return;
		}

		//���݃u�����h�������̏ꍇ�͋����I�ɏI��
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

		//�u�����h���������邩�ǂ���
		if (prevPlayAnim_.modelId != nowPlayAnim_.modelId)
		{
			blendFlag_ = true;
			blendRate_ = 0.0f;
		}
	}
}

void AnimationController::Stop()
{
	//�u�����h���̏ꍇ
	//���݂̃A�j���[�V�����̑O�ɍĐ����Ă����A�j�����~����
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