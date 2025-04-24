#pragma once
#include"../Manager/SceneManager.h"
#include "SceneBase.h"

class Stage;
class Camera;
class SkyDome;
class ActorBase;
class Sound;
class Effect;
class StopScene;
class AboutGameScene;
class Fade;

class GameScene :public SceneBase
{
public:

	//���݂̃Q�[���V�[���̏��
	enum class SCENE_STATE
	{
		START,
		PLAY,
		END,
	};

	//�R���X�g���N�^
	GameScene(SceneManager::CHARA_ID player, SceneManager::CHARA_ID enemy);

	//�f�X�g���N�^
	~GameScene(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;
	//�������J��
	void Release(void)override;

private:

	//��������(�b�P��)
	static constexpr float LIMIT_TIME = 120.0f;

	//�ő�J�E���g
	static constexpr float COUNT_DOWN_MAX = 3.9f;
	//�g�嗦�̍ő�ω����x
	static constexpr float EX_RATE_SPEED = 10.0f;
	//�����̊g�嗦(%)
	static constexpr float EX_DEFAULT_RATE = 380.0f;
	//�퓬�I�������̍ő�g�嗦
	static constexpr float FINISH_STR_RATE = 180.0f;
	//�퓬�I������
	static constexpr float END_MAX_TIME = 3.0f;

	//�Q�[�������G�N�X�g��
	int imgExplainEx_;
	//�t�F�C�h
	std::unique_ptr<Fade>fader_;

	//�v���C���[ID
	SceneManager::CHARA_ID playerId_;
	//�GID
	SceneManager::CHARA_ID enemyId_;

	//�f�o�b�O�p�̃v���C���[�C���X�^���X
	std::shared_ptr<ActorBase>player_;
	//�f�o�b�O�p�̃X�e�[�W�C���X�^���X
	std::unique_ptr<Stage>stage_;
	//�X�J�C�h�[��
	std::unique_ptr<SkyDome>skyDome_;
	//�f�o�b�O�p�̃G�l�~�[�C���X�^���X
	std::shared_ptr<ActorBase>enemy_;

	//�T�E���hBGM
	std::weak_ptr<Sound>gameSound_;
	//���艹
	std::weak_ptr<Sound>decisionSod_;
	//�������艹
	std::weak_ptr<Sound>finishSod_;
	//�J�E���g�_�E����
	std::weak_ptr<Sound>CountDownSod_;
	//�X�^�[�g��
	std::weak_ptr<Sound>StartSod_;


	//�c�莞��
	float remainTime_;

	//�|�[�Y���邩
	bool poseFlag_;
	std::unique_ptr<StopScene>pose_;

	//�Q�[���������
	bool aboutFlag_;
	std::unique_ptr<AboutGameScene>aboutGameScene_;

	//�X�^�[�g���o�̎��̃J�E���g�_�E��
	float countDown_;
	float prevCountDown_;
	//�����̊g�嗦
	float strExRate_;
	//�퓬�X�^�[�g�t���O
	bool startFlag_;
	//�퓬�I�����烊�U���g��ʂ܂ł̌v������
	float endTime_;

	//�G�t�F�N�g�F�퓬�J�n!
	std::weak_ptr<Effect>goEff_;

	using UpdateFunc = std::function<void(void)>;
	using DrawFunc = std::function<void(void)>;
	//�X�V�֐��|�C���^
	UpdateFunc update_;
	//�`��֐��|�C���^
	DrawFunc draw_;
	//���݂̃Q�[���V�[���̏��
	SCENE_STATE nowSceneStat_;



	//�����ł����g��Ȃ��֐�
	//��ԑJ��
	void ChangeState(const SCENE_STATE& stat);
	//�J��������
	void SynchroCamera(void);
	//�c�莞�Ԃ̕`��
	void DrawLimitTime(void);
	//�������ԏ���
	void SubRemainTime(void);

	//�퓬�J�n���o
	void UpdateStart(void);
	void DrawStart(void);
	//�퓬��
	void UpdatePlay(void);
	void DrawPlay(void);
	//�퓬�I�����o
	void UpdateEnd(void);
	void DrawEnd(void);

};