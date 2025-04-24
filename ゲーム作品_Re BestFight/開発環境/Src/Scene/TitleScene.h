#pragma once
#include "SceneBase.h"
#include<memory>
#include<functional>

class Transform;
class ActorBase;
class AnimationController;
class MenyuScene;
class Sound;

class TitleScene : public SceneBase
{
public:

	//���[�h�Z���N�g
	enum class SELECT
	{
		PLAY_GAME,
		ABOUT_GAME,
		SWITCH,
		MAX,
	};

	//�V�[��
	enum class IN_SCENE_ID
	{
		NONE,
		TITLE_ID,
		MENYU_ID,
	};

	//�J�[�\���̊g��
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr int CARSOL_WSIZE_X = 50;
	static constexpr int CARSOL_WSIZE_Y = 46;
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr int CARSOL_FSIZE_X = 100;
	static constexpr int CARSOL_FSIZE_Y = 93;

	//�����̊g��
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr int PLAYABOUT_WSIZE_X = 470;
	static constexpr int PLAYABOUT_WSIZE_Y = 60;
	static constexpr int SWITCH_WSIZE_X = 45;
	static constexpr int SWITCH_WSIZE_Y = 45;
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr int PLAYABOUT_FSIZE_X = 940;
	static constexpr int PLAYABOUT_FSIZE_Y = 121;
	static constexpr int SWITCH_FSIZE_X = 90;
	static constexpr int SWITCH_FSIZE_Y = 90;

	//���f���̑傫��
	static constexpr Size3DF TITLE_MODEL_SCALE = {1.0f,1.0f,1.0f};

	//���f���̍��W
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr Position3DF PLAYER_1_WPOS = { -500.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_2_WPOS = { 1300.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_3_WPOS = { 500.0f, -500.0f, 800.0f };
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr Position3DF PLAYER_1_FPOS = { -200.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_2_FPOS = { 1800.0f, -200.0f, 800.0f };
	static constexpr Position3DF PLAYER_3_FPOS = { 850.0f, -500.0f, 800.0f };
	//���f���̃T�C�Y
	//�E�B���h�E���[�h�̏ꍇ
	static constexpr float PLAYER_WSIZE = 3.0f;
	//�t���X�N���[�����[�h�̏ꍇ
	static constexpr float PLAYER_FSIZE = 3.5f;


	//�Z���N�g�F�v���C�Q�[���̃}�b�v���W
	static constexpr int PLAYGAME_MAP_POSX = 3;
	static constexpr int PLAYGAME_MAP_POSY = 5;
	//�Z���N�g�F�Q�[�������̃}�b�vY���W
	static constexpr int ABOUT_MAP_POSX = 3;
	static constexpr int ABOUT_MAP_POSY = 7;
	//�Z���N�g�F�d���{�^���̃}�b�v���W
	static constexpr int SWITCH_MAP_POSX = 15;
	static constexpr int SWITCH_MAP_POSY = 9;

	//�R���X�g���N�^
	TitleScene(void);

	//�f�X�g���N�^
	~TitleScene(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;
	//�������J��
	void Release(void)override;

private:

	//�A�N�^�[�������o������
	static constexpr int ACTOR_STEP_MAX = 100;
	//�f���������܂ł̎���
	static constexpr float DEMO_PLAY_TIME = 30.0f;
	
	//�d���{�^��
	int imgSwitch_;
	//�^�C�g���w�i
	int imgBack_;
	//�^�C�g���摜�̃n���h�����i�[
	int imgTitleHandle_;
	//�Z���N�g�摜�̃n���h�����i�[385,520
	int imgSelectHandle_;
	//PlayGame�摜
	int imgPlayGameHandle_[2];
	//AboutGame�摜
	int imgAboutGameHandle_[2];
	//���͂��Ă��邩
	std::map<SELECT, bool>inputSelect_;
	//���ݍ��킹�Ă���J�[�\��
	SELECT nowCarsolSelect_;
	//�J�[�\���̈ʒu���Ƃ̍��W(2D)
	Position2D carsolMapPos_[INT_CAST(SELECT::MAX)];
	//�J�[�\���̃T�C�Y
	Size2D carsolSize_;
	//�����̃T�C�Y
	Size2D playAboutSize_;
	//�d���{�^���̑傫��
	Size2D switchSize_;

	//�i����
	std::unique_ptr<Transform>plTrans1_;
	std::unique_ptr<AnimationController>plAnim1_;
	//���m
	std::unique_ptr<Transform>plTrans2_;
	std::unique_ptr<AnimationController>plAnim2_;
	//���p�t
	std::unique_ptr<Transform>plTrans3_;
	std::unique_ptr<AnimationController>plAnim3_;

	//�T�E���hBGM
	std::weak_ptr<Sound>titleSound_;
	//���j���[�V�[��
	std::unique_ptr<MenyuScene>menyu_;

	//�X�V�֐��|�C���^
	using UpdateFunc = std::function<void(void)>;
	using DrawFunc = std::function<void(void)>;
	UpdateFunc update_;
	DrawFunc draw_;

	//���݂̃V�[��
	IN_SCENE_ID nowScene_;
	//�f���v���C�Đ��܂ł̃J�E���^�[
	float demoCounter_;
	//���艹
	std::weak_ptr<Sound> decisionSod_;



	//�����ł����g��Ȃ��֐�
	//������
	void InitCarsol(void);
	void InitPlayAboutSwitch(void);

	//�V�[���؂�ւ�
	void ChangeScene(const IN_SCENE_ID& id);

	//�^�C�g���X�V
	void UpdateTitle(void);
	//���j���[�X�V
	void UpdateMenyu(void);
	//�^�C�g���`��
	void DrawSceneTitle(void);
	//���j���[�`��
	void DrawSceneMenyu(void);

	//�^�C�g���摜��`��
	void DrawTitle(void);
	//�J�[�\���摜��`��
	void DrawSelect(void);
	//�����摜
	void DrawPlayAboutSwitch(void);

};