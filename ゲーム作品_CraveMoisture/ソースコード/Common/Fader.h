#pragma once
#include <functional>

class Fader
{
private:

	// �t�F�[�h�̏��
	enum class FADER_STATE
	{
		NONE,
		FADE_OUT,	//�Ó]
		FADE_IN,	//���]
	};

public:

	// �R���X�g���N�^
	Fader(void);
	// �f�X�g���N�^
	~Fader(void);

	// �X�V
	void Update(void);
	// �`��
	void Draw(void);

	// �t�F�[�h�̊J�n
	void FadeStart(void);

	// ��ʂ����S�ɈÂ���
	const bool IsScreenBrack(void)const;

	// �t�F�[�h�A�E�g����
	const bool IsFadeOut(void)const;

	// �t�F�[�h�̏I�����
	const bool IsFadeEnd(void)const;

private:

	// �t�F�[�h�̍X�V
	using FADER_UPDATE = std::function<void(void)>;

private:

	// �t�F�[�h�̏��
	FADER_STATE nowState_;

	// �t�F�[�h�̍X�V�֐��|�C���^
	FADER_UPDATE faderUpdate_;

	// �����x
	float alpha_;

	// �Ó]���Ɍ��ɕ\������e�N�X�`��
	int fadeBackTexture_;


	//�������Ȃ�
	//�t�F�C�h�̏�Ԃ�ύX
	void ChangeFadeState(const FADER_STATE& state);

	// �Ó]���̔w�i�`��
	void DrawFadeBack(void);

	void UpdateFadeNone(void);
	//�t�F�C�h�A�E�g
	void UpdateFadeOut(void);
	//�t�F�C�h�C��
	void UpdateFadeIn(void);

};