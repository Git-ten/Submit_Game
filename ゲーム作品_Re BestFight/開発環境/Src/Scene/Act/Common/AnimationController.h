#pragma once
#include<map>
#include<string>

class AnimationController
{
public:

	//�A�j���[�V�����̏��
	struct ANIM_INFO
	{
		int modelId = -1;
		float speed = 0.0f;
		int attachNo = -1;
		float totalTime = 0.0f;
		float step = 0.0f;
		bool loopFlag = false;

		float start = 0.0f;
		float end = 0.0f;
	};

	//�u�����h�̐i�s���x
	static constexpr float BLEND_SPEED = 0.15f;
	
	//�R���X�g���N�^
	AnimationController(int& model);

	//�f�X�g���N�^
	~AnimationController(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

	//�A�j���[�V�����̒ǉ�
	//�g�p����A�j���[�V���������炩���ߓo�^���Ă���
	void Add(int type, const std::string& line, float speed, float start, float end, bool loop);

	//�ŏ��ɍĐ�����A�j���[�V����
	void Start(int type);

	//�A�j���[�V�����̍Đ�
	void Play(int type);
	//�Đ����̃A�j���[�V�����̒�~
	void Stop(void);

	void ClearAnimMap(void);

	//�A�j���[�V�������Đ����Ă��邩�擾
	bool IsPlayAnim(void)const;

	//���ݍĐ����Ă���A�j���[�V�����̃X�e�b�v�^�C��
	float GetStepAnimTime()const;

	//�u�����h���Ă��邩�̎擾
	bool IsBlendFlag(void)const;

private:

	using AnimationsData = std::map<int, ANIM_INFO>;

	//�A�j���[�V������o�^����
	AnimationsData animation_;

	//���ݍĐ����Ă���A�j���[�V����
	ANIM_INFO nowPlayAnim_;
	//�؂�ւ��O�ɍĐ����Ă����A�j���[�V����
	ANIM_INFO prevPlayAnim_;

	//�u�����h��������
	bool blendFlag_;
	//�u�����h��
	float blendRate_;

	//�A�j���[�V�������Đ����Ă��邩
	bool playAnimFlag_;

	//�A�^�b�`����Ώۃ��f��
	int& modelId_;

};

