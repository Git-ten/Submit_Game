#pragma once
#include "../../Scene/SceneBase.h"
#include "SceneID.h"
#include <memory>

class SceneStatus
{
public:

	// �R���X�g���N�^
	SceneStatus(void);
	//�f�X�g���N�^
	~SceneStatus(void) = default;

	// ���݂̃V�[���̏�����
	void InitScene();
	// ���݂̃V�[���̍X�V
	void UpdateScene(void);
	// ���݂̃V�[���̕`��
	void DrawScene(void);

	// ���݂̃V�[��ID���擾
	const SceneID::SCENE_ID& GetNowSceneState(void)const;

	// �V�[���̐؂�ւ�
	void ChangeSceneState(const SceneID::SCENE_ID& id);

private:

	// ���݂̃V�[��
	SceneID::SCENE_ID nowSceneId_;

	// �V�[��
	std::unique_ptr<SceneBase>scene_;

};