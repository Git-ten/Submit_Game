#pragma once
#include "../MenyuBase.h"
class TitleMenyu : public MenyuBase
{
public:

	TitleMenyu(void);
	~TitleMenyu(void)override;

	// �f�[�^�̃��[�h
	void LoadSceneData(void)override;

	bool IsCollMenyu(void);

	Position2D GetPos(void)override;

private:

	// �Z���N�g���
	enum class SELECT_KIND
	{
		NONE,
		NO,
		YES,
		MAX,
	};

	// ���j���[�g
	std::unique_ptr<Image>imgMenu_;

	// ���j���[���́u�`���[�g���A���ɐi�݂܂����H�v����
	std::unique_ptr<Image>imgTutorial_;

	// ���j���[���̌��݃Z���N�g���Ă��镔����\���~
	std::unique_ptr<Image>imgCircle_;
	// ���ݑI�𒆂̎��
	SELECT_KIND selectKind_;


	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	void InitOpenMenyu(void)override;

	// �T�E���h�̃����`���[��������
	void InitSoundPlayLaunch(void)override;

	// �w�i�摜�̕`��
	void DrawBack(void)override;

};

