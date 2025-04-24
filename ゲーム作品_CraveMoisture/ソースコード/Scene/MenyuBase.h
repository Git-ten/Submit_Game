#pragma once
#include "SceneBase.h"
class MenyuBase : public SceneBase
{
public:

	MenyuBase(void);
	virtual ~MenyuBase(void);

	// �f�[�^�̃��[�h
	virtual void LoadSceneData(void)override;

	void SetMenyuIsOpen(bool isOpen);

	bool IsOpen(void);

	virtual Position2D GetPos(void);

protected:


	// ���ʐ��̍�������������
	virtual void InitSpecial(void)override;

	// ���ʐ��̍����X�V
	virtual void UpdateSpecial(void)override;

	virtual void InitOpenMenyu(void);

	// �T�E���h�̃����`���[��������
	virtual void InitSoundPlayLaunch(void)override;

	int InputSelectMenyu(bool inputLeft, bool inputRight, int nowSelect, const int start, const int end);

	// �w�i�摜�̕`��
	virtual void DrawBack(void)override;

	// ���ʐ��̍����`��
	virtual void DrawSpecial(void)override;

};