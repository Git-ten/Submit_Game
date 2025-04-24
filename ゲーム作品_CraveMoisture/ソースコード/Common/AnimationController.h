#pragma once
#include<map>
#include<string>
#include "../Common/DataLoad/DataAnimation.h"


class AnimationController
{
public:

	//�R���X�g���N�^
	AnimationController(const int& model);
	//�f�X�g���N�^
	~AnimationController(void);

	//�X�V
	void Update(bool isSlow = false);


	//�A�j���[�V�����̒ǉ�
	//�g�p����A�j���[�V���������炩���ߓo�^���Ă���
	void Add(int type, const DataAnimation& data);


	//�ŏ��ɍĐ�����A�j���[�V����
	void Start(int type);


	//�A�j���[�V�����̍Đ�
	void Play(int type);
	//��~���̃A�j���[�V�����̍Đ�
	void RePlay(void);
	//�Đ����̃A�j���[�V�����̒�~
	void Stop(void);


	//�o�^�������ׂẴA�j���[�V�����A�z�z����N���A����
	void ClearAnimMap(void);


	//�A�j���[�V�������Đ����Ă��邩�擾
	const bool IsPlayAnim(void)const;


	//���ݍĐ����Ă���A�j���[�V�����̃X�e�b�v�^�C��
	const float GetStepAnimTime(void)const;
	// ���ݍĐ����Ă���A�j���[�V�����̃X�e�b�v�^�C���̎Q��
	float& GetReferenceStepAnimTime(void);
	//�A�j���[�V�����X�e�b�v�������̂Ƃ���܂ŃX�L�b�v
	void SkipAnimTime(float skipStep);
	// ���ݍĐ����Ă���A�j���[�V�����̏I���X�e�b�v���擾
	const float GetEndStepAnimTime(void)const;
	// ���ݍĐ����Ă���A�j���[�V�����̃A�^�b�`�ԍ�
	const int GetAttachAnimIndex(void)const;


	//�u�����h���Ă��邩�̎擾
	const bool IsBlendFlag(void)const;
	//�u�����h�����������I��
	void EndBlend(void);


	//�Đ����x���Z�b�g
	void SetPlaySpeed(float speed);
	// �Đ����x�������{����
	void MultPlaySpeed(const float mul);
	//�Đ����x���f�t�H���g�ɂ���
	void DefaultPlaySpeed(void);

private:

	//�A�j���[�V�����̏��
	struct ANIMATION_INFO
	{
		int nowType;
		std::string animationPath;	//�A�j���[�V�����̃p�X
		int modelId;				//���f��ID
		float speed;				//�A�j���[�V�������x
		int attachNo;				//�A�j���[�V�����̃A�^�b�`�ԍ�
		float totalTime;			//�A�j���[�V�����̏I������
		float step;					//���݂̃A�j���[�V�����X�e�b�v
		bool isLoop;				//�A�j���[�V���������[�v���邩�ۂ�
		float start;				//�A�j���[�V�����̃X�^�[�g�X�e�b�v
		float end;					//�A�j���[�V�����̏I���X�e�b�v
	};

	//�A�j���[�V�����f�[�^
	using AnimationsData = std::map<int, ANIMATION_INFO>;

private:

	//�A�j���[�V������o�^����
	AnimationsData animation_;


	//���ݍĐ����Ă���A�j���[�V����
	ANIMATION_INFO nowAnimation_;
	//�؂�ւ��O�ɍĐ����Ă����A�j���[�V����
	ANIMATION_INFO prevAnimation_;


	//�u�����h��������
	bool blendFlag_;
	//�u�����h��
	float blendRate_;


	//�A�j���[�V�������Đ����Ă��邩
	bool playAnimationFlag_;


	//�A�^�b�`����Ώۃ��f��
	const int& modelId_;


	//�Đ����x
	float animSpeed_;


	// �u�����h����
	void BlendAnimation(void);
	// �A�j���[�V��������
	void Animation(void);

	// �A�j���[�V�����Đ��̃Z�b�g�A�b�v
	void SetUpPlayAnimation(int type);

};

