#pragma once
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../GUI/Movie.h"
#include "../SceneBase.h"


class ResultScene : public SceneBase
{
public:

	ResultScene(void);
	~ResultScene(void)override;

	// �f�[�^�̃��[�h
	void LoadSceneData(void)override;

private:

	// �uTime�v�摜
	std::unique_ptr<Image>imgResultTime_;
	// �uDamage�v�摜
	std::unique_ptr<Image>imgResultDamage_;

	// ���ŗ�������
	std::unique_ptr<Movie> playerMovie_;

	// ���U���g��Փx�����摜
	std::unique_ptr<Image>imgResultLevel_;

	SceneManager::GameSceneToResultScene gameToResultData_;

	// ���ʐ��̍�������������
	void InitSpecial(void)override;

	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	// �w�i�摜�̕`��
	void DrawBack(void)override;

	// ���ʐ��̍����`��
	void DrawSpecial(void)override;

};