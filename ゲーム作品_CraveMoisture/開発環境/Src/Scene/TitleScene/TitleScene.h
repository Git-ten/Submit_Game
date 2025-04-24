#pragma once
#include "../GUI/Image.h"
#include "../GUI/ImageGUI.h"
#include "../../Common/Time.h"
#include "../SceneBase.h"
#include "TitleMenyu.h"

class TitleScene : public SceneBase
{
public:

	// �R���X�g���N�^
	TitleScene(void);
	// �f�X�g���N�^
	~TitleScene(void)override;

	// �f�[�^�̃��[�h
	void LoadSceneData(void)override;

private:

	// �S�̔w�i
	std::unique_ptr<Image>imgBack_;

	// �^�C�g�����摜
	std::unique_ptr<Image> imgTitleName_;

	// PushAnyButton�摜
	std::unique_ptr<Image> imgPushAnyButton_;

	// �V���b�g�_�E���L�[�摜
	std::unique_ptr<Image>imgShutDownKey_;

	// ���Ԍv��
	std::unique_ptr<Time> pixcelTime_;

	std::unique_ptr<TitleMenyu>titleMenyu_;

	// ���ʐ��̍�������������
	void InitSpecial(void)override;


	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	// PixcelRenderer�̏�����
	void InitPixcelRenderer(void)override;
	// PixcelRenderer�̍X�V
	void UpdatePixcelRenderer(void)override;
	// �T�E���h�̃����`���[��������
	void InitSoundPlayLaunch(void)override;


	// BGM�̍Đ�����
	void PlayBgm(void)override;

	// PushAnyButton�̃A�j���[�V��������
	void PushAnyButtonAnimation(void);


	// ���ʐ��̍����`��
	void DrawSpecial(void)override;

	// �w�i�摜�̕`��
	void DrawBack(void)override;
	// TitleName�̕`��
	void DrawTitleName(void);
	// PushAnyButton�̕`��
	void DrawPushAnyButton(void);

};