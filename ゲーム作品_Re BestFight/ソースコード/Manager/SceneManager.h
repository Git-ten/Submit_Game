#pragma once
#include <map>
#include <chrono>
#include<memory>

class SceneBase;
class Fade;
class Camera;
class Transform;

class SceneManager
{

public:

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,//�^�C�g��
		GAME,//�Q�[��
		SELECT,//�L�����Z���N�g
		RESULT,//���U���g
		DEMO,//�f��
		ABOUT,//�Q�[������
	};
	
	//�L�����N�^�[ID
	enum class CHARA_ID
	{
		NONE,
		FIGHTER_ID,//�i����ID
		SORDMAN_ID,//���mID
		MAGICIAN_ID,//�����mID
		MAX,
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	//������
	void Init(void);
	void Init3D(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);
	// ���\�[�X�̔j��
	void Destroy(void);

	// ��ԑJ��
	void ChangeScene(const SCENE_ID& nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetSceneID(void)const;

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	//�v���C���[�ƓG�̃Z�b�g
	void SetPlayerAndEnemy(CHARA_ID player, CHARA_ID enemy);

	//CPU���[�h�̃Z�b�g
	void SetCpuFlag(bool flg);

	//CPU���[�h�̎擾
	bool GetCpuFlag(void)const;

	//�X�N���[���̎擾
	int GetScreen(void)const;

	//��ʂ̐U�������Z�b�g
	void SetVibrationCnt(int count);

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	//���݂̃V�[��ID
	SCENE_ID sceneId_;

	// �e��V�[��
	std::unique_ptr<SceneBase>scene_;

	//�t�F�C�h
	std::unique_ptr<Fade>fader_;

	//�v���C���[
	CHARA_ID playerID_;
	//�G
	CHARA_ID enemyID_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	//CPU�Ɛ키��
	bool cpuFlag_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//�X�N���[���̐U����
	int vibrationCnt_;

	//���C���X�N���[��
	int mainScreen_;



	//�����ł̂ݎg�p����֐�
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager(void);

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);
	// �V�[���J��
	void DoChangeScene(const SCENE_ID& sceneId);
	//�t�F�[�h����(�蓮)
	void FadeManual(void);
	//�L������2�̂������_���őI��
	void RandomChara(CHARA_ID* chara_1, CHARA_ID* chara_2);
	//���C���X�N���[���̕`��
	void DrawMainScreen(void);

};