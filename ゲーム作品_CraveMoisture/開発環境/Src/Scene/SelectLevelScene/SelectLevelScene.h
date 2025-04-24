#pragma once
#include "SelectLevelMenyu.h"
#include "../SceneBase.h"

class SelectLevelScene : public SceneBase
{
public:

	SelectLevelScene(void);
	~SelectLevelScene(void)override;

	// �f�[�^�̃��[�h
	virtual void LoadSceneData(void);

private:

	std::unique_ptr<SelectLevelMenyu>selectMenyu_;

	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;


	// ���ʐ��̍����`��
	void DrawSpecial(void)override;


};