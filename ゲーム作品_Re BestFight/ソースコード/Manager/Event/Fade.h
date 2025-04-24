#pragma once
#include<functional>

class Fade
{
public:

	//�t�F�C�h�̏��
	enum class FADE_STATE
	{
		NONE,
		FADE_OUT,//�Ó]
		FADE_IN, //���]
	};

	//�R���X�g���N�^
	Fade(void);
	//�f�X�g���N�^
	~Fade(void);

	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

	//�t�F�C�h�̊J�n
	void FadeStart(void);
	void FadeStart(const FADE_STATE& state);

	//�t�F�C�h�̏�Ԃ�ύX
	void ChangeFadeState(const FADE_STATE& state);

	//�t�F�C�h�̑��x���Z�b�g
	void SetFadeSpeed(float speed);
	//�t�F�C�h�̑��x���f�t�H���g�ɖ߂�
	void DefaultFadeSpeed();

	//�t�F�C�h�����ǂ������擾
	bool IsFadeFlag(void);

	//�t�F�C�h�I�����Z�b�g
	void SetIsEndFlag(bool flg);
	//�t�F�C�h���I�����Ă��邩
	bool IsEndFlag(void);

	//�t�F�C�h�̌��݂̏�Ԃ��擾
	FADE_STATE GetNowState(void)const;

private:

	//�t�F�C�h�̑��x
	static constexpr float DEFAULT_FADE_SPEED = 10.0f;
	//�Ó]���̍ő�A���t�@�l
	static constexpr float ALPHA_MAX = 255.0f;
	//���]���̍ŏ��A���t�@�l
	static constexpr float ALPHA_MIN = 0.0f;

	using FadeFunc = std::function<void(void)>;

	FadeFunc fadeUpdate_;

	//�t�F�C�h��
	bool IsFadeFlag_;

	//�t�F�C�h���I�����邩
	bool IsEndFade_;

	//�Ó]�E���]���蓮�ōs����
	bool IsManualFlag_;

	//�t�F�C�h�̏��
	FADE_STATE state_;

	//�A���t�@�l
	float alpha_;

	//�t�F�C�h�̑��x
	float speed_;



	//�����ł̂ݎg�p����֐�
	//�������Ȃ�
	void UpdateFadeNone(void);
	//�t�F�C�h�A�E�g
	void UpdateFadeOut(void);
	//�t�F�C�h�C��
	void UpdateFadeIn(void);

};