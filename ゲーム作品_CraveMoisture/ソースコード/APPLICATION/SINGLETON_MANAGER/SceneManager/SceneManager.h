#pragma once
#include "../../../Common/DeltaTime.h"
#include "../../../Common/Camera.h"
#include "../../../Common/Fader.h"
#include "SceneStatus.h"

class SceneManager
{
public:

	struct SelectSceneToGameScene
	{
		float enemyLevel = 0.5f;// 0.5�`0.7�`1.0�̊Ԃ̂ǂꂩ
	};

	struct GameSceneToResultScene
	{
		float resultTime = 0.0f;
		float resultDamage = 0.0f;
		ResourceManager::RESOURCE_ID levelResource = ResourceManager::RESOURCE_ID::LEVEL_EASY;
	};

public:

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	// ������
	void Init(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// �V�[���̍X�V�����b�N
	void LockSceneUpdate(void);
	// �V�[���̍X�V���A�����b�N
	void UnLockSceneUpdate(void);
	// �V�[���̍X�V���b�N��Ԃ��擾
	const bool IsLockSceneUpdate(void)const;

	// �V�[���̐؂�ւ�
	void ChangeSceneState(const SceneID::SCENE_ID& id);

	// �Q�[���I���̐M��(true�ŏI��)
	const bool IsEndGame(void)const;
	// �Q�[���̏I������
	void EndGame(void);

	// �f���^�^�C���̎擾
	const float GetDeltaTime(void)const;

	// NULL�ł��邩
	static const bool IsNullptr(void);

	// �J�����̎Q�Ƃ��擾
	const std::weak_ptr<Camera>GetCamera(void)const;

	//���C�g�̌������擾
	const Dir3DF& GetLightDir(void)const;

	void SetUpSelectToGameScene(const SelectSceneToGameScene& data);
	SelectSceneToGameScene GetSelectToGameSceneData(void);

	void SetUpGameToResultScene(const GameSceneToResultScene& data);
	GameSceneToResultScene GetGameToResultSceneData(void);

private:

	SelectSceneToGameScene toGameSceneData_;
	GameSceneToResultScene toResultSceneData_;

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	// �V�[���̃X�e�[�^�X
	std::unique_ptr<SceneStatus>sceneStatus_;

	// �J����
	std::shared_ptr<Camera>camera_;

	// �t�F�[�h
	std::unique_ptr<Fader>fader_;

	//�Q�[���̏I�����
	bool isEndGame_;

	// �f���^�^�C��
	DeltaTime deltaTime_;

	//���C�g�̌���
	Dir3DF lightDir_;

	// �t�F�[�h�̋N��
	void FadeStart(void);

	// private�ŃR���X�g���N�^�𐶐�����
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	//�f�X�g���N�^
	~SceneManager(void) = default;

};