#pragma once
#include "../MenyuBase.h"
class SelectLevelMenyu : public MenyuBase
{
public:

	SelectLevelMenyu(void);
	~SelectLevelMenyu(void)override;

	// �f�[�^�̃��[�h
	void LoadSceneData(void)override;

private:

	// �Z���N�g���
	enum class SELECT_KIND
	{
		NONE,
		EASY,
		NORMAL,
		HARD,
		MAX,
	};

	// ���ݑI�𒆂̎��
	SELECT_KIND selectKind_;

	std::unique_ptr<Image>imgSelectLevelBack_;

	std::unique_ptr<Image>imgEnemyIcon_;

	std::unique_ptr<Image>imgCircle_;

	// ���ʐ��̍�������������
	void InitSpecial(void)override;

	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	// �w�i�摜�̕`��
	void DrawBack(void)override;

};