#pragma once
#include"../Manager/SceneManager.h"
#include "SceneBase.h"

class Stage;
class Camera;
class SkyDome;
class ActorBase;
class Sound;
class StopScene;

class DemoScene :public SceneBase
{
public:

	//�R���X�g���N�^
	DemoScene(SceneManager::CHARA_ID player, SceneManager::CHARA_ID enemy);

	//�f�X�g���N�^
	~DemoScene(void)override;

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
	static constexpr float LIMIT_TIME = 40.0f;

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

	//�c�莞��
	float remainTime_;



	//�����ł����g��Ȃ��֐�
	//�J��������
	void SynchroCamera(void);
	//�c�莞�Ԃ̕`��
	void DrawLimitTime(void);
	//�������ԏ���
	void SubRemainTime(void);

};