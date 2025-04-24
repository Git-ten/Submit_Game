#pragma once
#include "../../Scene/GUI/ImageDiv.h"
#include "../MenyuBase.h"

class PauseMenyu : public MenyuBase
{
public:

	PauseMenyu(void);
	~PauseMenyu(void);

	// �f�[�^�̃��[�h
	void LoadSceneData(void)override;

private:

	// �Z���N�g���
	enum class SELECT_KIND
	{
		NONE,
		RETURN_TITLE,
		RETURN_GAME,
		MAX,
	};

	// �|�[�Y�w�i
	std::unique_ptr<Image>imgPauseBack_;

	// �uTime�v�摜
	std::unique_ptr<Image>imgTime_;
	// �uDamage�v�摜
	std::unique_ptr<Image>imgDamage_;
	// ���j���[���̌��݃Z���N�g���Ă��镔����\���~
	std::unique_ptr<Image>imgCircle_;

	// ���ݑI�𒆂̎��
	SELECT_KIND selectKind_;


	// ���ʐ��̍�������������
	void InitSpecial(void)override;

	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	void InitOpenMenyu(void)override;

	// �T�E���h�̃����`���[��������
	void InitSoundPlayLaunch(void)override;

	// �w�i�摜�̕`��
	void DrawBack(void)override;
};