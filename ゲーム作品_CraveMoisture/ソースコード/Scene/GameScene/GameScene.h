#pragma once
#include <vector>
#include <unordered_set>
#include "../../Renderer/ModelRenderer/ModelShadowRenderer.h"
#include "../../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include "../SceneBase.h"
#include "../../ACTOR/ActorBase.h"
#include "BattleManager.h"
#include "PauseMenyu.h"

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);
	// �f�X�g���N�^
	virtual ~GameScene(void)override;

	void Release(void)override;

	// �f�[�^�̃��[�h
	virtual void LoadSceneData(void)override;

protected:

	// �S�ẴA�N�^�[�̐�
	static constexpr long ALL_ACTOR_NUM = 4;

	enum ACTOR_ID
	{
		NONE = 1 << 0,
		PLAYER = 1 << 1,
		ENEMY = 1 << 2,
		STAGE = 1 << 3,
		SKYDOME = 1 << 4,
	};

	enum BATTLE_STEP
	{
		NONE_BATTLE,
		BATTLE_START,
		BATTLE,
		BATTLE_WIN,
		BATTLE_GAMEOVER,
	};

	// �V�[���̃X�g�b�v
	bool isSceneStop_;

	BATTLE_STEP battleStep_;
	std::unique_ptr<BattleManager>battleManager_;

	std::unique_ptr<PauseMenyu>pause_;

	// �o�^����Ă���ACTOR�͂��ׂē����蔻�菈�������
	ACTOR_ID actorCollisionId_;

	std::vector<std::unique_ptr<ActorBase>>actors_;

	std::vector<std::shared_ptr<Image>>actorsUi_;

	// �o�g���I�����Ɏ��̃V�[���ֈڍs����܂ł̃^�C�}�[
	Time nextSceneTime_;

	// �Q�[���I�[�o�[�����摜
	std::unique_ptr<Image>imgGameOver_;



	// ���[�f�B���O��������
	bool isLoadData_;

	// ���[�h�w�i�摜
	std::unique_ptr<Image> imgLoadBack_;

	//���[�h�Q�[�W
	std::unique_ptr<Image> imgLoadGage_;
	// ���[�h�Q�[�W�̘g
	std::unique_ptr<Image> imgLoadGageCase_;
	//�c��Q�[�W�i���o�[�摜
	std::unique_ptr<ImageDiv> imgLoadPercent_;
	//�P�ʋL��('%'�������g��)
	std::unique_ptr<ImageDiv> imgLoadSymbol_;

	// �t�F�[�h
	std::unique_ptr<Fader>fader_;

	// �c��Q�[�W(�p�[�Z���g�P��)
	float loadGageRate_;
	// �c��Q�[�W�̍ő吔(���[�h���ɕϓ�)
	float loadGageRateMax_;

	// �Q�[���V�[�����X�g�b�v
	void StopGameScene(void);
	// �Q�[���V�[�����Đ�
	void PlayGameScene(void);


	// �Q�[�W���A�b�v
	void AddGagePercent(const float rate);
	// �Q�[�W�̍ő�l���������ǉ�
	void AddGagePercentMax(const float rateMax);


	virtual void SetUpActors(void);

	// ���ʐ��̍�������������
	void InitSpecial(void)override;

	virtual void InitAddSpecial(void);

	// �e�V�F�[�_�̏�����
	void InitShadowShader(void)override;

	virtual void InitBattleManager(void);

	// ���ʐ��̍����X�V
	void UpdateSpecial(void)override;

	virtual void UpdateAddSpecial(void);

	void SetUpEnemyLevelResource(void);

	void InitActors(void);
	void SetUpActorsCollision(void);
	void SetCollision(long& nowCounter, const long size, bool isCanSetCollision);

	void SetUpActorsUi(void);

	// �T�E���h�̃����`���[��������
	virtual void InitSoundPlayLaunch(void)override;
	void PlayBgm(void)override;

	void Pause(void);

	void DrawPercent(void);

	// ���ʐ��̍����`��
	void DrawSpecial(void)override;

	virtual void DrawAddSpecial(void);

	// ���ׂẴG�t�F�N�g�̕`�揈��
	void DrawAllEffect(void);

};